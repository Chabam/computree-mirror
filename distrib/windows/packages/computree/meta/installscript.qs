var ComponentSelectionPage = null;

function cancelInstaller(message)
{
    installer.setDefaultPageVisible(QInstaller.Introduction, false);
    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
    installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);

    var abortText = "<font color='red'>" + message +"</font>";
    installer.setValue("FinishedText", abortText);
}

function Component() {
    // When installing
    if (installer.isInstaller()) {
		// System check (architecture, OS, version)
        if (systemInfo.currentCpuArchitecture != "x86_64") {
            cancelInstaller("Installation on " + systemInfo.currentCpuArchitecture + " architecture is not supported by Computree.");
            return;
        }
        if (systemInfo.productType != "windows") {
            cancelInstaller("Installation on " + systemInfo.productType + " is not supported by Computree.");
            return;
        }
        var str = systemInfo.productVersion;
        if (parseInt(str.substr(0,str.indexOf(' ')),10) < 7) {
            cancelInstaller("Installation on Windows " + systemInfo.productVersion + " is not supported by Computree.");
            return;
        }

        // Add custom installation mode page
        component.loaded.connect(component, Component.prototype.installerLoaded);
        ComponentSelectionPage = gui.pageById(QInstaller.ComponentSelection);

        // BUG : on first page, the default component page is still visible, and the new page is not
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
		installer.addWizardPage(component, "InstallationWidget", QInstaller.ComponentSelection);

        // Desactivate several plugins that are not ready (to be updated once ready)
        installer.componentByName("library.F").setValue("Virtual", "true");
        installer.componentByName("library.G").setValue("Virtual", "true");
        installer.componentByName("plugin.F").setValue("Virtual", "true");
        installer.componentByName("plugin.H").setValue("Virtual", "true");
        installer.componentByName("plugin.I").setValue("Virtual", "true");
        installer.componentByName("plugin.J").setValue("Virtual", "true");
        installer.componentByName("plugin.K").setValue("Virtual", "true");
		
		// For final actions
        installer.installationFinished.connect(component, Component.prototype.installationFinishedPageIsShown);
        installer.finishButtonClicked.connect(component, Component.prototype.installationFinished);
    }

    if(installer.isUninstaller()) {
        installer.uninstallationFinished.connect(component, Component.prototype.uninstallationFinished);            
    }
}

Component.prototype.installerLoaded = function () {
    var widget = gui.pageWidgetByObjectName("DynamicInstallationWidget");
    if (widget != null) {
        widget.customInstall.toggled.connect(this, Component.prototype.customInstallToggled);
        widget.defaultInstall.toggled.connect(this, Component.prototype.defaultInstallToggled);
        widget.completeInstall.toggled.connect(this, Component.prototype.completeInstallToggled);

        widget.defaultInstall.checked = true;
        widget.windowTitle = qsTr("Select Installation Type");
    }
}

Component.prototype.customInstallToggled = function (checked) {
    if (checked) {
        if (ComponentSelectionPage != null)
            ComponentSelectionPage.selectDefault();
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, true);
    }
}

Component.prototype.defaultInstallToggled = function (checked) {
    if (checked) {
        if (ComponentSelectionPage != null)
            ComponentSelectionPage.selectDefault();
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    }
}

Component.prototype.completeInstallToggled = function (checked) {
    if (checked) {
        if (ComponentSelectionPage != null)
            ComponentSelectionPage.selectAll();
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    }
}

function getComponent( name ) {
    var comps=installer.components();
    for( i=0; i< comps.length; i++ ) {
        if( comps[i].name == name ) return comps[i];
    }
    throw new Error( "Component not found: " + name );
}

function isComponentSelected( name ) {
    try{ return getComponent( name ).installationRequested(); }
    catch(e){ console.log( "Component not found: " + name ); }
    return false;
}

Component.prototype.createOperations = function() {
    // call default implementation to actually install something
    component.createOperations();

    // Additionnal shortcuts
	if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/CompuTreeGui.cmd", "@StartMenuDir@/CompuTree.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/CompuTreeGui.exe", "iconId=0",
			"description=Forest inventory with Lidar (ONF)");
	    component.addOperation("CreateShortcut", "@TargetDir@/CompuTreeGuiUninstall.exe", "@StartMenuDir@/Uninstall.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/System32/shell32.dll",
            "iconId=31", "description=Modify/Update/Uninstall program features");
		component.addOperation("CreateShortcut", "@TargetDir@/Licenses/gnu-lgpl-v3.0.txt", "@StartMenuDir@/License.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/System32/shell32.dll",
            "iconId=1", "description=Software license");
		component.addOperation("CreateShortcut", "@TargetDir@/Computree_Charter_v2018_08_eng.pdf", "@StartMenuDir@/Chart.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/System32/shell32.dll",
            "iconId=165", "description=Software chart");
		if (isComponentSelected("computree.B")) {
            // BUG : not recognized as a directory but as a file link
		    component.addOperation("CreateShortcut", "@TargetDir@/examples", "@StartMenuDir@/Samples.lnk",
                "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/shell32.dll",
                "iconId=3", "description=Samples directory");
		}
    }
}

Component.prototype.installationFinishedPageIsShown = function() {
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "FinishedWidget", QInstaller.InstallationFinished );
            var checkboxForm = component.userInterface( "FinishedWidget" );
            if (!isComponentSelected("computree.C")) {
                checkboxForm.checkBox_msvc.enabled = false;
            }
			checkboxForm.checkBox_msvc.stateChanged.connect(component, Component.prototype.customMsvcToggled);
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.customMsvcToggled = function (checked) {
	var checkboxForm = component.userInterface( "FinishedWidget" );
	checkboxForm.checkBox_run.enabled = !checked;
}

Component.prototype.installationFinished = function() {
    try {
		// Final checkbox options
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var checkboxForm = component.userInterface( "FinishedWidget" );
            if (checkboxForm && checkboxForm.checkBox_shortcut.checked) {
                installer.executeDetached("cmd", ["/C", "copy", "@StartMenuDir@\\CompuTree.lnk", "@DesktopDir@"]);
            }
            if (checkboxForm && checkboxForm.checkBox_msvc.checked && checkboxForm.checkBox_msvc.enabled) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/vc_redist.x64.exe");
            }
			if (checkboxForm && checkboxForm.checkBox_chart.checked) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/Computree_Charter_v2018_08_eng.pdf");
            }
            if (checkboxForm && checkboxForm.checkBox_run.checked && checkboxForm.checkBox_run.enabled) {
                QDesktopServices.openUrl("file:///" + installer.value("TargetDir") + "/CompuTreeGui.cmd");
            }
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.uninstallationFinished = function() {
	// BUG : uninstaller does not execute the final commad to erase the desktp shortcut
    installer.executeDetached("cmd", ["/C", "del", "/F", "/Q", "@DesktopDir@\\CompuTree.lnk"]);
}
