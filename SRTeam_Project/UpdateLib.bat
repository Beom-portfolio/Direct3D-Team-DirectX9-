
//// 복사명령어 옵션(/)  복사 할 파일이 있는경로(.) 복사받을 폴더의 경로(.)

xcopy /y .\Engine\Headers\*.h .\Reference\Headers\
xcopy /y .\Engine\System\Headers\*.h .\Reference\Headers\


xcopy /y .\Engine\System\Bin\*.lib .\Reference\Librarys\

xcopy /y .\Engine\System\Bin\*.dll .\Client\Bin\


xcopy /y .\Engine\Utility\Headers\*.h .\Reference\Headers\

xcopy /y .\Engine\Utility\Bin\*.lib .\Reference\Librarys\

xcopy /y .\Engine\Utility\Bin\*.dll .\Client\Bin\


xcopy /y .\Engine\System\Bin\*.dll .\MapTool_SR\Bin\

xcopy /y .\Engine\Utility\Bin\*.dll .\MapTool_SR\Bin\