object Form1: TForm1
  Left = 708
  Top = 374
  BorderStyle = bsSingle
  Caption = 'STM32 HID Control Panel'
  ClientHeight = 316
  ClientWidth = 452
  Color = clWhite
  DockSite = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object HeaderPanel: TPanel
    Left = 0
    Top = 0
    Width = 452
    Height = 58
    Align = alTop
    BevelOuter = bvNone
    Color = 15790320
    TabOrder = 0
    object TitleLabel: TLabel
      Left = 16
      Top = 10
      Width = 202
      Height = 19
      Caption = 'STM32 HID Control Panel'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object DeviceLabel: TLabel
      Left = 17
      Top = 34
      Width = 192
      Height = 13
      Caption = 'BlackPill USB HID command interface'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object CalButton: TButton
      Left = 368
      Top = 16
      Width = 68
      Height = 25
      Caption = 'Cal'
      TabOrder = 0
      OnClick = CalButtonClick
    end
  end
  object ChannelsGroup: TGroupBox
    Left = 16
    Top = 72
    Width = 420
    Height = 176
    Caption = ' Channels '
    TabOrder = 1
    object Channel1Label: TLabel
      Left = 42
      Top = 24
      Width = 72
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'CH 1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Channel2Label: TLabel
      Left = 174
      Top = 24
      Width = 72
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'CH 2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Channel3Label: TLabel
      Left = 306
      Top = 24
      Width = 72
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'CH 3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Up1: TSpeedButton
      Tag = 101
      Left = 42
      Top = 42
      Width = 72
      Height = 28
      Caption = '+'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Up2: TSpeedButton
      Tag = 102
      Left = 174
      Top = 42
      Width = 72
      Height = 28
      Caption = '+'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Up3: TSpeedButton
      Tag = 103
      Left = 306
      Top = 42
      Width = 72
      Height = 28
      Caption = '+'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Lab1: TLabel
      Left = 42
      Top = 76
      Width = 72
      Height = 23
      Alignment = taCenter
      AutoSize = False
      Caption = '54'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Lab2: TLabel
      Left = 174
      Top = 76
      Width = 72
      Height = 23
      Alignment = taCenter
      AutoSize = False
      Caption = '26'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Lab3: TLabel
      Left = 306
      Top = 76
      Width = 72
      Height = 23
      Alignment = taCenter
      AutoSize = False
      Caption = '30'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Dw1: TSpeedButton
      Tag = 201
      Left = 42
      Top = 104
      Width = 72
      Height = 28
      Caption = '-'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Dw2: TSpeedButton
      Tag = 202
      Left = 174
      Top = 104
      Width = 72
      Height = 28
      Caption = '-'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Dw3: TSpeedButton
      Tag = 203
      Left = 306
      Top = 104
      Width = 72
      Height = 28
      Caption = '-'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Sw1: TSpeedButton
      Tag = 301
      Left = 42
      Top = 138
      Width = 72
      Height = 24
      Caption = 'OFF'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Sw2: TSpeedButton
      Tag = 302
      Left = 174
      Top = 138
      Width = 72
      Height = 24
      Caption = 'OFF'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
    object Sw3: TSpeedButton
      Tag = 303
      Left = 306
      Top = 138
      Width = 72
      Height = 24
      Caption = 'OFF'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = SpeedButtonClick
    end
  end
  object StatusGroup: TGroupBox
    Left = 16
    Top = 256
    Width = 420
    Height = 36
    Caption = ' Status '
    TabOrder = 2
    object StatusHintLabel: TLabel
      Left = 12
      Top = 16
      Width = 194
      Height = 13
      Caption = 'Auto reconnect: VID 1209 / PID 0001'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SendHintLabel: TLabel
      Left = 292
      Top = 16
      Width = 112
      Height = 13
      Alignment = taRightJustify
      Caption = 'Range: 0..100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object StaticText1: TStaticText
    Left = 0
    Top = 296
    Width = 452
    Height = 20
    Align = alBottom
    BorderStyle = sbsSunken
    Caption = 'Waiting for BlackPill...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object TimerReconnect: TTimer
    OnTimer = TimerReconnectTimer
    Left = 384
    Top = 16
  end
  object TimerRead: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerReadTimer
    Left = 352
    Top = 16
  end
  object ControlTimer: TTimer
    Interval = 100
    OnTimer = ControlTimerTimer
    Left = 320
    Top = 16
  end
end
