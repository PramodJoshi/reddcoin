#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include "walletmodel.h"

#include <QDialog>
#include <QImage>
#include <QLabel>

namespace Ui {
    class ReceiveRequestDialog;
}
class OptionsModel;

/* Label widget for QR code. This image can be dragged, dropped, copied and saved
 * to disk.
 */
class QRImageWidget : public QLabel
{
    Q_OBJECT

public:
    explicit QRImageWidget(QWidget *parent = 0);
    QImage exportImage();

public slots:
    void saveImage();
    void copyImage();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
};

class ReceiveRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveRequestDialog(const SendCoinsRecipient &info, QWidget *parent = 0);
    ~ReceiveRequestDialog();

    void setModel(OptionsModel *model);

private slots:
    void on_lnReqAmount_textChanged();
    void on_lnLabel_textChanged();
    void on_lnMessage_textChanged();

    void updateDisplayUnit();

private:
    Ui::ReceiveRequestDialog *ui;
    OptionsModel *model;
    SendCoinsRecipient info;

    void genCode();
    QString getURI();
};

#endif // QRCODEDIALOG_H