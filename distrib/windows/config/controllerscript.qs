function Controller()
{
}

Controller.prototype.FinishedPageCallback = function() {
	if (installer.isUninstaller() && installer.status == QInstaller.Success)
    {
		installer.executeDetached("cmd", ["/C", "del", "/F", "/Q", "@DesktopDir@\\CompuTree.lnk"]);
	}
}
