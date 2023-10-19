#include <QThread>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QSemaphore>
#include <QMutex>

class WriteThread : public QThread
{
    Q_OBJECT

public:
    WriteThread(QFile &file, qint64 dataSize, qint64 startPosition)
            : file(file), dataSize(dataSize), startPosition(startPosition)
        {
        }

protected:
    void run() override
    {
        if (file.isOpen())
        {
            QByteArray buffer(1024 * 1024, 'A'); // Adjust the buffer size as needed
            qint64 bytesWritten = 0;

            for (qint64 i = 0; i < dataSize; i += buffer.size())
            {
                qint64 bytesToWrite = qMin(static_cast<qint64>(buffer.size()), dataSize - i);

                file.seek(startPosition + i);
                qint64 bytesWrittenThisIteration = file.write(buffer, bytesToWrite);
                if (bytesWrittenThisIteration == -1)
                {
                    qDebug() << "Failed to write to file.";
                    break;
                }
                bytesWritten += bytesWrittenThisIteration;
            }

            file.flush(); // Ensure data is flushed to disk
        }
        else
        {
            qDebug() << "File is not open for writing.";
        }
    }

private:
    QFile &file;
    qint64 dataSize;
    qint64 startPosition;
};

class WriterThreadSemaphore : public QThread
{
    Q_OBJECT

public:
    WriterThreadSemaphore(const QString &filePath, qint64 dataSize, QByteArray *buffer, QSemaphore *semaphore, QMutex *mutex)
        : filePath(filePath), dataSize(dataSize), buffer(buffer), semaphore(semaphore), mutex(mutex)
    {
    }

protected:
    void run() override
    {
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly))
        {
            qint64 totalBytesToWrite = dataSize;
            qint64 bytesWritten = 0;
            QDateTime startTime = QDateTime::currentDateTime();

            while (bytesWritten < totalBytesToWrite)
            {
                qint64 bytesToWrite = qMin(static_cast<qint64>(buffer->size()), totalBytesToWrite - bytesWritten);

                QMutexLocker locker(mutex);
                file.write(buffer->data(), static_cast<int>(bytesToWrite));

                bytesWritten += bytesToWrite;
            }

            QDateTime endTime = QDateTime::currentDateTime();
            qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

            double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
            qDebug() << "Thread Write Speed: " << writeSpeed << "MB/s";

            file.close();
        }
        else
        {
            qDebug() << "Failed to open the file for writing.";
        }

        // Release the semaphore to signal completion
        semaphore->release();
    }

private:
    QString filePath;
    qint64 dataSize;
    QByteArray *buffer;
    QSemaphore *semaphore;
    QMutex *mutex;
};
