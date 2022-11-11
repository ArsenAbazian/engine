@rem Если у вас установлен более новый фрэймворк замените 50215 на подверсию имеющегося у вас фрэйворка.
@set FrameworkVersion=50727


@echo Uses Framework Version %FrameworkVersion%
@set MsBuildPath=%SystemRoot%\Microsoft.NET\Framework\v2.0.%FrameworkVersion%
@set MsBuild=%MsBuildPath%\MSBuild.exe 

%MsBuild% Rsdn.Editor.sln       /property:Configuration=Debug
