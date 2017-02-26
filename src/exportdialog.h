#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QProgressDialog>

/**
 * @brief Defines an export dialog that shows the current
 * progress of an export operation. Note that the value is between 0-100,
 * and the setValue() is the ENTIRE progress.
 */
class ExportDialog : public QProgressDialog
{
public:
    ExportDialog(QWidget* parent, int count);
    void incrementItem();

private:
    int count;
    int currentItem = 0;
};

#endif // EXPORTDIALOG_H
