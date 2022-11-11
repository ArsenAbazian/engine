SET Root=Rsdn.Editor

echo test

RD %Root%\bin /S /Q
RD %Root%\obj /S /Q

SET Root=Rsdn.Editor.Test

RD %Root%\bin /S /Q
RD %Root%\obj /S /Q

SET Root=Rsdn.Editor.UnitTest

RD %Root%\bin /S /Q
RD %Root%\obj /S /Q

rem del Rsdn.Editor.suo /Q /A:H