FrontColor = RGB(0,0,200)
Width = 16
Height = 16

CreateImage(1,Width+1,Height+1,32,0)

StartDrawing(ImageOutput(1))
LineXY(0,Height/2,Width/2,0,FrontColor)
LineXY(Width/2,0,Width,Height/2, FrontColor)
LineXY(Width,Height/2,Width/2,Height,FrontColor)
LineXY(Width/2,Height,0,Height/2,FrontColor)

FillArea(Width/2,Height/2,FrontColor, FrontColor)

StopDrawing()

SaveImage(1,"start.bmp", #PB_ImagePlugin_BMP)
; IDE Options = PureBasic 5.51 (Linux - x64)
; CursorPosition = 16
; EnableXP
; CompileSourceDirectory
; EnableCompileCount = 8
; EnableBuildCount = 0
; EnableExeConstant