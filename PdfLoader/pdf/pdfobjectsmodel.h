#pragma once

/** QT headers **/
#include <QAbstractListModel>
#include <QByteArray>
#include <QMap>
#include <QString>

/*
	Model representing the objects in a pdf file.
*/
class PdfObjectsModel : public QAbstractListModel {
	Q_OBJECT
	public:
		PdfObjectsModel(QObject *parent=0);

		void load(QString filename);
		void loadPdf(QByteArray data);

		int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

		QByteArray object(int object_id) const;
		QString printableObject(int object_id) const;

		qint32 objectId(int index) const;
		qint32 objectIndex(int object_id) const;

	private:
		void extractObjects();

		QList<QByteArray> pages();
		QByteArray extractStream(QByteArray object);
		QByteArray stripComments(QByteArray data);

		int objectId(QByteArray data) const;
		QByteArray objectContent(QByteArray data) const;

		int current_object_id, current_object_ref;
		QByteArray m_data;
		QMap<qint32, QByteArray> m_object_streams;
		QMap<qint32, QByteArray> m_objects;

		QString printableDictionary(QByteArray dictionary) const;

		bool isDictionaty(int object_id) const;

		bool isCatalog(int object_id) const;
		bool isPage(int object_id) const;
		bool isContent(int object_id) const;

		QString objectType(int object_id) const;

		/**
			Get pages in catalog

			@param catalog_object_id Object id for catalog.
		  */
		QList<qint32> pages(int catalog_object_id) const;

		QMap<qint32, QString> m_object_types;

};
