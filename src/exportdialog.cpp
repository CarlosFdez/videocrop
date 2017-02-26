#include "exportdialog.h"

ExportDialog::ExportDialog(QWidget* parent, int count)
    : QProgressDialog(parent)
{
    this->count = count;
    setSizeGripEnabled(false);
    setWindowModality(Qt::WindowModal);
    setRange(0, 100);

    this->incrementItem();
}

void ExportDialog::incrementItem()
{
    currentItem++;

    // update export dialog with progress
    QString labelText;
    if (currentItem > count)
        labelText = "Finished exporting";
    else
        labelText = QString("Exporting %1 of %2").arg(currentItem).arg(count);

    setLabelText(labelText);
    repaint();
}

