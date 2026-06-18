unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes,  Graphics, Controls, Forms,
  Dialogs,  StdCtrls,  hid_raw, ExtCtrls, Buttons, ToolWin, ComCtrls;


  type
  THidReaderThread = class(TThread)
  private
    FText: string;
    procedure SyncLog;
  protected
    procedure Execute; override;
  end;

type
  TForm1 = class(TForm)
    TimerReconnect: TTimer;
    TimerRead: TTimer;
    Up1: TSpeedButton;
    Up2: TSpeedButton;
    Up3: TSpeedButton;
    Dw1: TSpeedButton;
    Dw2: TSpeedButton;
    Dw3: TSpeedButton;
    Lab1: TLabel;
    Lab2: TLabel;
    Lab3: TLabel;
    Sw1: TSpeedButton;
    Sw2: TSpeedButton;
    Sw3: TSpeedButton;
    ControlTimer: TTimer;
    StaticText1: TStaticText;
    HeaderPanel: TPanel;
    TitleLabel: TLabel;
    DeviceLabel: TLabel;
    CalButton: TButton;
    ChannelsGroup: TGroupBox;
    Channel1Label: TLabel;
    Channel2Label: TLabel;
    Channel3Label: TLabel;
    StatusGroup: TGroupBox;
    StatusHintLabel: TLabel;
    SendHintLabel: TLabel;
    procedure SpeedButtonClick(Sender: TObject);
    procedure btnSendClick(Sender: TObject);
    procedure TimerReconnectTimer(Sender: TObject);
    procedure TimerReadTimer(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure CalButtonClick(Sender: TObject);
    procedure ControlTimerTimer(Sender: TObject);
    procedure ProcessRxPacket(const Buf: array of Byte);
  private
    { Private declarations }
    FReader: THidReaderThread;
    procedure StartReader;
    procedure StopReader;
    procedure SendGuiStateToSTM32;
    procedure SendCommandToSTM32(Cmd: Byte);
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

  s1 :boolean = false;
  s2 :boolean = false;
  s3 :boolean = true;
  d1 :integer = 11;
  d2 :integer = 22;
  d3 :integer = 33;
  resend: boolean = false;
  calibrationMode: boolean = false;
  pingCounter: integer = 0;

  FRxPacket: array[0..7] of Byte;

implementation

{$R *.dfm}

//---------------------------------------------
procedure THidReaderThread.SyncLog;
begin
  Form1.StaticText1.Caption:=FText;
  Form1.ProcessRxPacket(FRxPacket);
end;
//---------------------------------------------
procedure THidReaderThread.Execute;
var
  Buf: array[0..8] of Byte;
  S: string;
  I: Integer;
  P: array[0..7] of Byte;
begin
  while not Terminated do
  begin
    FillChar(Buf, SizeOf(Buf), 0);

   if HidReadBlocking(Buf, SizeOf(Buf)) then
begin
  S := 'RX: ';

  for I := 0 to 8 do
    S := S + IntToHex(Buf[I], 2) + ' ';

  FText := S;

  Move(Buf[1], FRxPacket[0], 8);
  Synchronize(SyncLog);
end
    else
    begin
      FText := 'BlackPill disconnected';
      Synchronize(SyncLog);
      Break;
    end;
  end;
end;
//---------------------------------------------
procedure TForm1.StartReader;
begin
  if FReader <> nil then Exit;

  FReader := THidReaderThread.Create(True);
  FReader.FreeOnTerminate := True;
  FReader.Resume;
end;
//---------------------------------------------
procedure TForm1.StopReader;
begin
  if FReader <> nil then
  begin
    FReader.Terminate;
    HidClose;
    FReader := nil;
  end;
end;
//---------------------------------------------
procedure TForm1.SendGuiStateToSTM32;
var
  Buf: array[0..8] of Byte;
begin
  FillChar(Buf, SizeOf(Buf), 0);

  Buf[0] := 0;
  Buf[1] := $02;
  Buf[2] := d1;
  Buf[3] := d2;
  Buf[4] := d3;
  Buf[5] := ord(s1);
  Buf[6] := ord(s2);
  Buf[7] := ord(s3);
  Buf[8] := Buf[1] + Buf[2] + Buf[3] + Buf[4] + Buf[5] + Buf[6] + Buf[7];

  if HidWrite(Buf, SizeOf(Buf)) then
    StaticText1.Caption:='Write OK'
  else
    StaticText1.Caption:='Write FAIL: ' + HidLastError;
end;

//---------------------------------------------
procedure TForm1.SendCommandToSTM32(Cmd: Byte);
var
  Buf: array[0..8] of Byte;
begin
  FillChar(Buf, SizeOf(Buf), 0);

  Buf[0] := 0;
  Buf[1] := Cmd;
  Buf[8] := Buf[1] + Buf[2] + Buf[3] + Buf[4] + Buf[5] + Buf[6] + Buf[7];

  if HidWrite(Buf, SizeOf(Buf)) then
    StaticText1.Caption := 'Command sent: ' + IntToHex(Cmd, 2)
  else
    StaticText1.Caption := 'Write FAIL: ' + HidLastError;
end;

//---------------------------------------------
procedure TForm1.btnSendClick(Sender: TObject);
begin
SendGuiStateToSTM32;
end;
//---------------------------------------------
procedure TForm1.TimerReconnectTimer(Sender: TObject);
begin
  if not HidIsOpen then
  begin
    if HidOpen($1209, $0001) then
    begin
      StaticText1.Caption:='BlackPill connected';
      StartReader;
    end;
  end;
end;
//---------------------------------------------

procedure TForm1.TimerReadTimer(Sender: TObject);
var
  Buf: array[0..8] of Byte;
  S: string;
  I: Integer;
begin
  if not HidIsOpen then Exit;
  FillChar(Buf, SizeOf(Buf), 0);
end;
//---------------------------------------------
procedure TForm1.FormCreate(Sender: TObject);
begin

FReader := nil;

end;

//---------------------------------------------
procedure TForm1.CalButtonClick(Sender: TObject);
begin
  calibrationMode := not calibrationMode;

  if calibrationMode then
  begin
    CalButton.Caption := 'Stop Cal';
    SendCommandToSTM32($10);
  end
  else
  begin
    CalButton.Caption := 'Cal';
    SendCommandToSTM32($11);
  end;
end;


//---------------------------------------------
procedure TForm1.SpeedButtonClick(Sender: TObject);
begin
  case (Sender as TSpeedButton).Tag of
    101: begin if d1 < 100 then inc(d1); resend:= true; end;
    201: begin if d1 > 0 then dec(d1);  resend:= true; end;

    102: begin if d2 < 100 then inc(d2); resend:= true; end;
    202: begin if d2 > 0 then dec(d2); resend:= true; end;

    103: begin if d3 < 100 then inc(d3); resend:= true; end;
    203: begin if d3 > 0 then dec(d3); resend:= true; end;

    301: begin S1 := not S1; resend:= true; end;
    302: begin S2 := not S2; resend:= true; end;
    303: begin S3 := not S3; resend:= true; end;
  end;

end;
//---------------------------------------------
procedure TForm1.ControlTimerTimer(Sender: TObject);
begin
Lab1.Caption := IntToStr(d1);
Lab2.Caption := IntToStr(d2);
Lab3.Caption := IntToStr(d3);

if s1 then begin Sw1.Caption := 'ON'; Sw1.Font.Color := clGreen; end
      else begin Sw1.Caption := 'OFF'; Sw1.Font.Color := clGrayText; end;
if s2 then begin Sw2.Caption := 'ON'; Sw2.Font.Color := clGreen; end
      else begin Sw2.Caption := 'OFF'; Sw2.Font.Color := clGrayText; end;
if s3 then begin Sw3.Caption := 'ON'; Sw3.Font.Color := clGreen; end
      else begin Sw3.Caption := 'OFF'; Sw3.Font.Color := clGrayText; end;

  if resend then  begin SendGuiStateToSTM32; resend:= false; end;

  Inc(pingCounter);
  if pingCounter >= 10 then
  begin
    pingCounter := 0;
    if HidIsOpen then
      SendCommandToSTM32($12);
  end;

end;
//---------------------------------------------
procedure TForm1.ProcessRxPacket(const Buf: array of Byte);
var
  Sum: Byte;
begin
  Sum := Buf[0] + Buf[1] + Buf[2] + Buf[3] + Buf[4] + Buf[5] + Buf[6];

  if Buf[7] <> Sum then
  begin
    Form1.StaticText1.Caption:='RX checksum error';
    Exit;
  end;

  case Buf[0] of

    $01, $7F:
    begin
      d1 := Buf[1];
      d2 := Buf[2];
      d3 := Buf[3];

      if buf[4] > 0 then S1:= true else s1:=false;
      if buf[5] > 0 then S2:= true else s2:=false;
      if buf[6] > 0 then S3:= true else s3:=false;
    end;

  end;
end;
//---------------------------------------------
end.
