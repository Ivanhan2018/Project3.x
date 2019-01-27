
@echo off

set curRootPath=%cd%\..
echo %curRootPath%

echo 当前目录
set curPath=%cd%

echo 源路径
set srcClientPath=%curPath%\Classes
echo %srcClientPath%

echo 请按任意键，开始拷贝！！！
pause

title 拷贝运行文件
color 30

xcopy %srcClientPath%\SysFunc\Source_Android\MyNSString.cpp %srcClientPath%\SysFunc\Source_Ios\MyNSString.mm /Y
goto end
xcopy %srcClientPath%\AppDelegate.cpp %srcClientPath%\AppDelegate.mm /Y
xcopy %srcClientPath%\entity\EntityMgr.cpp %srcClientPath%\entity\EntityMgr.mm /Y
xcopy %srcClientPath%\gui\PlayerInfoLayer.cpp %srcClientPath%\gui\PlayerInfoLayer.mm /Y
xcopy %srcClientPath%\gui\MissionLayer.cpp %srcClientPath%\gui\MissionLayer.mm /Y
xcopy %srcClientPath%\lottery\BetLimit.cpp %srcClientPath%\lottery\BetLimit.mm /Y
xcopy %srcClientPath%\lottery\RoomLayer.cpp %srcClientPath%\lottery\RoomLayer.mm /Y
xcopy %srcClientPath%\lottery\bjl\SceneView.cpp %srcClientPath%\lottery\bjl\SceneView.mm /Y
xcopy %srcClientPath%\lottery\bjl\BJLSceneControl.cpp %srcClientPath%\lottery\bjl\BJLSceneControl.mm /Y
xcopy %srcClientPath%\lottery\dzpk\DZPKLayer.cpp %srcClientPath%\lottery\dzpk\DZPKLayer.mm /Y
xcopy %srcClientPath%\lottery\ddz\DDZSceneView.cpp %srcClientPath%\lottery\ddz\DDZSceneView.mm /Y
xcopy %srcClientPath%\msgDispatch\MsgDispatch.cpp %srcClientPath%\msgDispatch\MsgDispatch.mm /Y
xcopy %srcClientPath%\BaseGame\ToolKit.cpp %srcClientPath%\BaseGame\ToolKit.mm /Y
:end
pause