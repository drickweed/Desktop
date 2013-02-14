/** QT headers **/
#include <QtCore/QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QStringList>
#include <QTemporaryFile>
#include <QDebug>
#include <QFileInfo>

/** STL headers **/
#include <iostream>
#include <assert.h>
#include <stdio.h>

/** Zlib **/
#include <zlib.h>

#define CHUNK 16384

using namespace std;

QString STREAM_START = "stream\n";
QString STREAM_END = "endstream\n";
int START_OF_FILE = 0;
int INVALID_INDEX = -1;


int inflate(FILE *source, FILE *dest)
{
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = fread(in, 1, CHUNK, source);
		if (ferror(source)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
				(void)inflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

int inflate(QString input, QString output) {
	FILE *in, *out;

	in = fopen(input.toStdString().c_str(), "rb");
	out = fopen(output.toStdString().c_str(), "wb");

	int result = inflate(in, out);

	fclose(in);
	fclose(out);

	return result;
}

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	if (app.arguments().count() == 1) {
		cout << "Specify pdf file name to extract streams from" << endl;
		return 1;
	}

	// Get file name
	QString filename = QFileInfo(app.arguments().at(1)).baseName();

	// Read pdf data into byte stream
	QFile pdf_file(app.arguments().at(1));
	pdf_file.open(QIODevice::ReadOnly);
	QByteArray pdf_data = pdf_file.readAll();
	pdf_file.close();

	int starting_search_position = START_OF_FILE; // Index in pdf byte data where last stream was found.
	int last_found_stream_index = 0;
	int streams_found = 0;

	// Find next stream
	while ((last_found_stream_index = pdf_data.indexOf(STREAM_START, starting_search_position)) != INVALID_INDEX) {

		last_found_stream_index = last_found_stream_index + STREAM_START.size();

		// Find end of stream
		int end_of_stream = pdf_data.indexOf(STREAM_END, last_found_stream_index);

		if (end_of_stream == INVALID_INDEX)
			break;

		streams_found++;

		int stream_length = end_of_stream - last_found_stream_index;

		// Extract stream
		QByteArray stream = pdf_data.mid(last_found_stream_index, stream_length);

		// Write extracted stream to temporary file

		// Alternative: Use a known permanent filename
		//QFile compressed_file(QString("compressed-%1.txt").arg(streams_found));
		//compressed_file.open(QIODevice::WriteOnly);

		QTemporaryFile compressed_file;

		compressed_file.open();
		compressed_file.write(stream);
		compressed_file.close();

		qDebug() << "Extracting" << compressed_file.fileName();

		int result = inflate(compressed_file.fileName(), QString("%1-%2.txt").arg(filename).arg(streams_found));
		if (result != Z_OK)
			qDebug() << "Error" << result << "extracting data from" << compressed_file.fileName();

		starting_search_position = end_of_stream + STREAM_END.size();
	}

	return 0;
}
