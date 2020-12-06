//
//  @file Language.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 23.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef LANGUAGE_H
#define LANGUAGE_H

#define GSTR(Name)		GetString(EString##Name)

enum EString {
	EStringInstall,
	EStringUninstall,
	EStringOK,
	EStringYes,
	EStringNo,
	EStringAdvanced,
	EStringBack,
	EStringNext,
	EStringCancel,
	EStringAccept,
	EStringContinue,
	EStringFinish,
	EStringFinished,
	EStringWelcome,
	EStringCompanyURL,
	EStringAdvancedConfigMode,
	EStringPleaseConfigureSettings,
	EStringLicenseAgreement,
	EStringPleaseReadLicense,
	EStringTypical,
	EStringConfigurationType,
	EStringPleaseSelectInstallationType,
	EStringInstallationHasBeenFinished,
	EStringHasBeenReinstalled,
	EStringHasBeenUpdated,
	EStringHasBeenRemoved,
	EStringPleaseLogoffToStartUsing,
	EStringPreparing,
	EStringInstallation,
	EStringReinstallation,
	EStringUpdating,
	EStringUninstalling,
	EStringPleaseWaitInstalling,
	EStringPleaseWaitReinstalling,
	EStringPleaseWaitUpdating,
	EStringPleaseWaitRemoving,
	EStringClickContinueToStart,
	EStringInstallerReadyToInstall,
	EStringInstallerReadyToReinstall,
	EStringInstallerReadyToUpdate,
	EStringInstallerReadyToUninstall,
	EStringReadyToStartInstallation,
	EStringReadyToReinstall,
	EStringReadyToUpdate,
	EStringReadyToRemove,
	EStringClickNextToContinue,
	EStringInstallerWillUpdate,
	EStringInstallerWillReinstall,
	EStringInstallerWillInstall,
	EStringOrClickUninstallToContinue,
	EStringCancelConfirmation,
	EStringReallyAbortInstallation,
	EStringPreparingWait,
	EStringExtractingFiles,
	EStringCopyingFiles,
	EStringRemovingFiles,
	EStringRegistering,
	EStringConfiguring,
	EStringFinalizing,
	EStringCommandFailed,
	EStringSelectLanguage,
	EStringUnableToUninstall,
};

int GetLanguage();
void SetLanguage(int iLanguage);
bool SetLanguage(const char *lpLanguage);

int GetLanguageCount();
const char *GetLanguageName(int iLanguage, bool fNative);

const char *GetString(EString eString);

#endif
