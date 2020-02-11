// Copyright (c) 2012-2019 The Vortex developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef VORTEX_QT_MULTISIGDIALOG_H
#define VORTEX_QT_MULTISIGDIALOG_H

//#include "multisigaddressentry.h"
//#include "multisiginputentry.h"
//#include "sendcoinsentry.h"
//#include "walletmodel.h"

#include <qt/platformstyle.h>

#include <QDialog>

class MultisigAddressEntry;
class MultisigInputEntry;
class WalletModel;
class SendCoinsEntry;

namespace Ui
{
    class MultisigDialog;
}

class MultisigDialog : public QDialog
{
    Q_OBJECT;

  public:
    explicit MultisigDialog(const PlatformStyle *_platformStyle, QWidget *parent = 0);
    ~MultisigDialog();
    void setModel(WalletModel *model);

  public Q_SLOTS:
    MultisigAddressEntry * addPubKey();
    void clear();
    void updateRemoveEnabled();
    MultisigInputEntry * addInput();
    SendCoinsEntry * addOutput();

private:
    Ui::MultisigDialog *ui;
    WalletModel *model;
    const PlatformStyle *platformStyle;

  private Q_SLOTS:
    void on_createAddressButton_clicked();
    void on_copyMultisigAddressButton_clicked();
    void on_copyRedeemScriptButton_clicked();
    void on_saveRedeemScriptButton_clicked();
    void on_saveMultisigAddressButton_clicked();
    void removeEntry(MultisigAddressEntry *entry);
    void on_createTransactionButton_clicked();
    void on_transaction_textChanged();
    void on_copyTransactionButton_clicked();
    void on_pasteTransactionButton_clicked();
    void on_signTransactionButton_clicked();
    void on_copySignedTransactionButton_clicked();
    void on_sendTransactionButton_clicked();
    void removeEntry(MultisigInputEntry *entry);
    void removeEntry(SendCoinsEntry *entry);
    void updateAmounts();
};

#endif // VORTEX_QT_MULTISIGDIALOG_H
