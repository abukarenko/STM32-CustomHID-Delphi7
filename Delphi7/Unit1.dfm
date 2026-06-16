object Form1: TForm1
  Left = 708
  Top = 374
  Width = 302
  Height = 213
  Caption = 'Form1'
  Color = clBtnFace
  DockSite = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Up1: TSpeedButton
    Tag = 101
    Left = 16
    Top = 16
    Width = 23
    Height = 22
    Caption = 'Up'
    OnClick = SpeedButtonClick
  end
  object Up2: TSpeedButton
    Tag = 102
    Left = 80
    Top = 16
    Width = 23
    Height = 22
    Caption = 'Up'
    OnClick = SpeedButtonClick
  end
  object Up3: TSpeedButton
    Tag = 103
    Left = 144
    Top = 16
    Width = 23
    Height = 22
    Caption = 'Up'
    OnClick = SpeedButtonClick
  end
  object Dw1: TSpeedButton
    Tag = 201
    Left = 16
    Top = 56
    Width = 23
    Height = 22
    Caption = 'Dw'
    OnClick = SpeedButtonClick
  end
  object Dw2: TSpeedButton
    Tag = 202
    Left = 80
    Top = 56
    Width = 23
    Height = 22
    Caption = 'Dw'
    OnClick = SpeedButtonClick
  end
  object Dw3: TSpeedButton
    Tag = 203
    Left = 144
    Top = 56
    Width = 23
    Height = 22
    Caption = 'Dw'
    OnClick = SpeedButtonClick
  end
  object Lab1: TLabel
    Left = 16
    Top = 40
    Width = 12
    Height = 13
    Caption = '54'
  end
  object Lab2: TLabel
    Left = 80
    Top = 40
    Width = 12
    Height = 13
    Caption = '26'
  end
  object Lab3: TLabel
    Left = 144
    Top = 40
    Width = 12
    Height = 13
    Caption = '30'
  end
  object Sw1: TSpeedButton
    Tag = 301
    Left = 16
    Top = 98
    Width = 23
    Height = 22
    Caption = 'Off'
    OnClick = SpeedButtonClick
  end
  object Sw2: TSpeedButton
    Tag = 302
    Left = 79
    Top = 98
    Width = 23
    Height = 22
    Caption = 'Off'
    OnClick = SpeedButtonClick
  end
  object Sw3: TSpeedButton
    Tag = 303
    Left = 142
    Top = 98
    Width = 23
    Height = 22
    Caption = 'Off'
    OnClick = SpeedButtonClick
  end
  object StaticText1: TStaticText
    Left = 0
    Top = 157
    Width = 286
    Height = 17
    Align = alBottom
    Caption = 'StaticText1'
    TabOrder = 0
  end
  object TimerReconnect: TTimer
    OnTimer = TimerReconnectTimer
    Left = 48
    Top = 32
  end
  object TimerRead: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerReadTimer
    Left = 112
    Top = 32
  end
  object ControlTimer: TTimer
    Interval = 100
    OnTimer = ControlTimerTimer
    Left = 48
    Top = 64
  end
end
