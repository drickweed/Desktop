#pragma once

/** QT headers **/
#include <QAbstractListModel>
#include <QList>
#include <QByteArray>
#include <QPoint>

/*
	Model of pdf graphics commands.
*/
class PdfCommandsModel : public QAbstractListModel {
    Q_OBJECT

    public:
        PdfCommandsModel(QObject *parent=0);

        /*
            Get 'count' commands from model.
        */
        QByteArray pdfFromCommands(int count) const;

        /*
            Load pdf data from file.

            @param path Path to file.
        */
        void load(QString path);

        void setCommands(QByteArray data);

        int rowCount(const QModelIndex &parent=QModelIndex()) const;

        QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

		QByteArray command(int row) const;

    private:
        QList<QByteArray> m_commands;


};

