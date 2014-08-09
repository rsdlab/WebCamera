@set rtc_name=WebCamera

@call :EXEC_RTC %rtc_name%

:EXEC_RTC
@cd .\build\src\release
@%1Comp.exe -f rtc.conf
