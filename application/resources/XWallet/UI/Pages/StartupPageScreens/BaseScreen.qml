import QtQuick

import XWallet.Style.Controls
import XWallet.Natives
import XWallet.Data

import XWallet.UI.Components

Control {
    id: idRoot

    required property StartupViewModel viewModel

    background: Item {}

    component StepErrorsToolTip : ErrorToolTip {
        id: idStepErrorsToolTip

        visible: errors > 0

        property int errors: idRoot.viewModel.step.errors

        model: ObjectModel {
            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.EmptyPassword
                text: qsTr("Password is empty")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.PasswordAlreadyExists
                text: qsTr("Password is too easy, please choose another")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.InvalidPassword
                text: qsTr("Password is invalid")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.InvalidMnemonicPassphrase
                text: qsTr("Mnemonic passphrase is invalid")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.InvalidKeyFilePath
                text: qsTr("Key file path is invalid")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.InvalidKeyFile
                text: qsTr("Key file is invalid")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.InvalidKeyFilePassword
                text: qsTr("Key file password is invalid")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.InvalidBiometricData
                text: qsTr("Biometric data is invalid")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.WrongStep
                text: qsTr("DEVELOPER: Wrong step")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.WrongData
                text: qsTr("DEVELOPER: Wrong step data")
            }

            ErrorToolTip.ErrorRow {
                trigger: idStepErrorsToolTip.errors & Step.DatabaseError
                text: qsTr("Database error")
            }
        }
    }
}
