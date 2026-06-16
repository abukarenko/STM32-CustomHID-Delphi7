unit hid_raw;

interface

uses
  Windows, SysUtils;

function HidOpen(VID, PID: Word): Boolean;
procedure HidClose;
function HidWrite(const Buf; Size: DWORD): Boolean;
function HidIsOpen: Boolean;
function HidLastError: string;
function HidReadBlocking(var Buf; Size: DWORD): Boolean;
function HidGetHandle: THandle;

implementation

const
  DIGCF_PRESENT         = $00000002;
  DIGCF_DEVICEINTERFACE = $00000010;

  FILE_DEVICE_UNKNOWN   = $00000022;
  FILE_ANY_ACCESS       = 0;
  METHOD_BUFFERED       = 0;

type
  HDEVINFO = Pointer;
  ULONG_PTR = Cardinal;

  SP_DEVICE_INTERFACE_DATA = packed record
    cbSize: DWORD;
    InterfaceClassGuid: TGUID;
    Flags: DWORD;
    Reserved: ULONG_PTR;
  end;
  PSP_DEVICE_INTERFACE_DATA = ^SP_DEVICE_INTERFACE_DATA;

  SP_DEVICE_INTERFACE_DETAIL_DATA_A = packed record
    cbSize: DWORD;
    DevicePath: array[0..1023] of AnsiChar;
  end;
  PSP_DEVICE_INTERFACE_DETAIL_DATA_A = ^SP_DEVICE_INTERFACE_DETAIL_DATA_A;

  HIDD_ATTRIBUTES = packed record
    Size: ULONG;
    VendorID: Word;
    ProductID: Word;
    VersionNumber: Word;
  end;
  PHIDD_ATTRIBUTES = ^HIDD_ATTRIBUTES;


var
  HidHandle: THandle = INVALID_HANDLE_VALUE;
  LastErr: string = '';
//==================================================
function SetupDiGetClassDevsA(ClassGuid: PGUID; Enumerator: PAnsiChar;
  hwndParent: HWND; Flags: DWORD): HDEVINFO; stdcall;
  external 'setupapi.dll' name 'SetupDiGetClassDevsA';
//==================================================
function SetupDiEnumDeviceInterfaces(DeviceInfoSet: HDEVINFO; DeviceInfoData: Pointer;
  const InterfaceClassGuid: TGUID; MemberIndex: DWORD;
  var DeviceInterfaceData: SP_DEVICE_INTERFACE_DATA): BOOL; stdcall;
  external 'setupapi.dll' name 'SetupDiEnumDeviceInterfaces';
//==================================================
function SetupDiGetDeviceInterfaceDetailA(DeviceInfoSet: HDEVINFO;
  DeviceInterfaceData: PSP_DEVICE_INTERFACE_DATA;
  DeviceInterfaceDetailData: PSP_DEVICE_INTERFACE_DETAIL_DATA_A;
  DeviceInterfaceDetailDataSize: DWORD;
  var RequiredSize: DWORD;
  DeviceInfoData: Pointer): BOOL; stdcall;
  external 'setupapi.dll' name 'SetupDiGetDeviceInterfaceDetailA';
//==================================================
function SetupDiDestroyDeviceInfoList(DeviceInfoSet: HDEVINFO): BOOL; stdcall;
  external 'setupapi.dll' name 'SetupDiDestroyDeviceInfoList';
//==================================================
procedure HidD_GetHidGuid(var HidGuid: TGUID); stdcall;
  external 'hid.dll' name 'HidD_GetHidGuid';
//==================================================
function HidD_GetAttributes(HidDeviceObject: THandle;
  var Attributes: HIDD_ATTRIBUTES): BOOL; stdcall;
  external 'hid.dll' name 'HidD_GetAttributes';
//==================================================
function HidIsOpen: Boolean;
begin
  Result := HidHandle <> INVALID_HANDLE_VALUE;
end;
//==================================================
function HidLastError: string;
begin
  Result := LastErr;
end;
//==================================================
procedure HidClose;
begin
  if HidHandle <> INVALID_HANDLE_VALUE then
  begin
    CloseHandle(HidHandle);
    HidHandle := INVALID_HANDLE_VALUE;
  end;
end;
//==================================================
function HidOpen(VID, PID: Word): Boolean;
var
  HidGuid: TGUID;
  DevInfo: HDEVINFO;
  IfData: SP_DEVICE_INTERFACE_DATA;
  Detail: SP_DEVICE_INTERFACE_DETAIL_DATA_A;
  RequiredSize: DWORD;
  Index: DWORD;
  H: THandle;
  Attr: HIDD_ATTRIBUTES;
begin
  Result := False;
  LastErr := '';

  HidClose;

  HidD_GetHidGuid(HidGuid);

  DevInfo := SetupDiGetClassDevsA(
    @HidGuid,
    nil,
    0,
    DIGCF_PRESENT or DIGCF_DEVICEINTERFACE
  );

  if DevInfo = Pointer(INVALID_HANDLE_VALUE) then
  begin
    LastErr := 'SetupDiGetClassDevs failed';
    Exit;
  end;

  try
    Index := 0;

    while True do
    begin
      ZeroMemory(@IfData, SizeOf(IfData));
      IfData.cbSize := SizeOf(IfData);

      if not SetupDiEnumDeviceInterfaces(
        DevInfo,
        nil,
        HidGuid,
        Index,
        IfData
      ) then
        Break;

      ZeroMemory(@Detail, SizeOf(Detail));

      // Для ANSI структуры cbSize обычно 5 на 32-bit Delphi
      Detail.cbSize := 5;

      RequiredSize := 0;

      if SetupDiGetDeviceInterfaceDetailA(
        DevInfo,
        @IfData,
        @Detail,
        SizeOf(Detail),
        RequiredSize,
        nil
      ) then
      begin
        H := CreateFileA(
        Detail.DevicePath,
        GENERIC_READ or GENERIC_WRITE,
        FILE_SHARE_READ or FILE_SHARE_WRITE,
        nil,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL ,
        0
        );

        if H <> INVALID_HANDLE_VALUE then
        begin
          ZeroMemory(@Attr, SizeOf(Attr));
          Attr.Size := SizeOf(Attr);

          if HidD_GetAttributes(H, Attr) then
          begin
            if (Attr.VendorID = VID) and (Attr.ProductID = PID) then
            begin
              HidHandle := H;
              Result := True;
              Exit;
            end;
          end;

          CloseHandle(H);
        end;
      end;

      Inc(Index);
    end;

    LastErr := 'Device not found';
  finally
    SetupDiDestroyDeviceInfoList(DevInfo);
  end;
end;
//==================================================
function HidWrite(const Buf; Size: DWORD): Boolean;
var
  Written: DWORD;
begin
  Result := False;
  LastErr := '';

  if HidHandle = INVALID_HANDLE_VALUE then
  begin
    LastErr := 'Device not open';
    Exit;
  end;

  Written := 0;

  Result := WriteFile(
    HidHandle,
    Buf,
    Size,
    Written,
    nil
  );

  if not Result then
    LastErr := SysErrorMessage(GetLastError)
  else if Written <> Size then
  begin
    LastErr := 'Partial write';
    Result := False;
  end;
end;
//==================================================
function HidReadBlocking(var Buf; Size: DWORD): Boolean;
var
  Readed: DWORD;
begin
  Result := False;
  LastErr := '';

  if HidHandle = INVALID_HANDLE_VALUE then
  begin
    LastErr := 'Device not open';
    Exit;
  end;

  Readed := 0;

  Result := ReadFile(
    HidHandle,
    Buf,
    Size,
    Readed,
    nil
  );

  if not Result then
  begin
    LastErr := SysErrorMessage(GetLastError);
    HidClose;
    Exit;
  end;
  if Readed <> Size then
  begin
    LastErr := 'Partial read';
    Result := False;
    Exit;
  end;
  Result := True;
end;
//==================================================
function HidGetHandle: THandle;
begin
  Result := HidHandle;
end;
//==================================================
end.
