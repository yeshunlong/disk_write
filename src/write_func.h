#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QBuffer>
#include <QSemaphore>
#include <QMutex>
#include <QDataStream>
#include <windows.h>
#include "multi_process.h"

double write_using_textstream_to_file()
{
    // 文件路径
    QString filePath = "write_using_textstream_to_file.dat";

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
        qint64 bytesWritten = 0;
        qint64 chunkSize = 1024 * 1024; // 1MB
        char buffer[chunkSize];

        QDateTime startTime = QDateTime::currentDateTime();

        while (bytesWritten < totalBytesToWrite)
        {
            qint64 bytesToWrite = qMin(chunkSize, totalBytesToWrite - bytesWritten);
            file.write(buffer, bytesToWrite);
            bytesWritten += bytesToWrite;
        }

        QDateTime endTime = QDateTime::currentDateTime();
        qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

        double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
        qDebug() << "Write Speed: " << writeSpeed << "MB/s";

        file.close();
        return writeSpeed;
    }
    else
    {
        qDebug() << "Failed to open the file for writing.";
    }
    return -1;
}

double write_using_writefile()
{
    // 文件路径
    QString filePath = "write_using_writefile.dat";

    // 使用Windows系统函数创建文件句柄
    HANDLE hFile = CreateFile(
        filePath.toStdWString().c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        qDebug() << "Failed to open the file for writing.";
        return -1;
    }

    qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
    qint64 bytesWritten = 0;
    qint64 chunkSize = 1024 * 1024; // 1MB
    char buffer[chunkSize];

    QDateTime startTime = QDateTime::currentDateTime();

    while (bytesWritten < totalBytesToWrite)
    {
        qint64 bytesToWrite = qMin(chunkSize, totalBytesToWrite - bytesWritten);
        DWORD bytesWrittenThisIteration;
        if (!WriteFile(hFile, buffer, static_cast<DWORD>(bytesToWrite), &bytesWrittenThisIteration, NULL))
        {
            qDebug() << "Failed to write to file.";
            break;
        }
        bytesWritten += bytesToWrite;
    }

    QDateTime endTime = QDateTime::currentDateTime();
    qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

    double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
    qDebug() << "Write Speed: " << writeSpeed << "MB/s";
    return writeSpeed;
}

double write_using_fwrite()
{
    // 文件路径
    QString filePath = "write_using_fwrite.dat";

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly))
    {
        qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
        qint64 bytesWritten = 0;
        qint64 chunkSize = 1024 * 1024; // 1MB
        char buffer[chunkSize];
        QDateTime startTime = QDateTime::currentDateTime();

        while (bytesWritten < totalBytesToWrite)
        {
            qint64 bytesToWrite = qMin(chunkSize, totalBytesToWrite - bytesWritten);
            qint64 bytesWrittenThisIteration = file.write(buffer, static_cast<int>(bytesToWrite));
            if (bytesWrittenThisIteration == -1)
            {
                qDebug() << "Failed to write to file.";
                break;
            }
            bytesWritten += bytesWrittenThisIteration;
        }

        QDateTime endTime = QDateTime::currentDateTime();
        qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

        double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
        qDebug() << "Write Speed: " << writeSpeed << "MB/s";

        file.close();
        return writeSpeed;
    }
    else
    {
        qDebug() << "Failed to open the file for writing.";
    }
}

double write_using_fwrite_diff_chunksize()
{
    // 文件路径
    QString filePath = "write_using_fwrite_diff_chunksize.dat";

    qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
    QDateTime startTime, endTime;
    std::vector<int> chunkSizes = {1024, 4096, 8192, 16384, 32768}; // Try different block sizes

    double maxWriteSpeed = 0.0;
    for (int chunkSize : chunkSizes)
    {
        FILE *file = fopen(filePath.toUtf8().constData(), "wb");

        if (file)
        {
            qint64 bytesWritten = 0;
            char buffer[chunkSize];
            startTime = QDateTime::currentDateTime();

            while (bytesWritten < totalBytesToWrite)
            {
                qint64 bytesToWrite = qMin(static_cast<qint64>(chunkSize), totalBytesToWrite - bytesWritten);

                size_t bytesWrittenThisIteration = fwrite(buffer, 1, bytesToWrite, file);
                if (bytesWrittenThisIteration < static_cast<size_t>(bytesToWrite))
                {
                    qDebug() << "Failed to write to file.";
                    break;
                }
                bytesWritten += bytesWrittenThisIteration;
            }

            endTime = QDateTime::currentDateTime();
            double elapsedMilliseconds = startTime.msecsTo(endTime);
            double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
            qDebug() << "Write Speed (Chunk Size " << chunkSize << "): " << writeSpeed << "MB/s";

            fclose(file);
            maxWriteSpeed = qMax(maxWriteSpeed, writeSpeed);
        }
        else
        {
            qDebug() << "Failed to open the file for writing.";
            return -1;
        }
    }
    return maxWriteSpeed;
}

double write_using_memory_map()
{
    // 文件路径
    QString filePath = "write_using_memory_map.dat";

    qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QDateTime startTime = QDateTime::currentDateTime();

        qint64 chunkSize = 1024 * 1024; // 1MB
        QByteArray buffer(chunkSize, 'A');

        // Memory-mapped file
        file.write(buffer); // Preallocate file space
        QBuffer bufferMapped;
        bufferMapped.open(QIODevice::ReadWrite);
        bufferMapped.write(buffer);
        file.write(bufferMapped.data());
        file.seek(0);

        qint64 bytesWritten = 0;

        while (bytesWritten < totalBytesToWrite)
        {
            qint64 bytesToWrite = qMin(chunkSize, totalBytesToWrite - bytesWritten);
            qint64 bytesWrittenThisIteration = file.write(buffer, static_cast<int>(bytesToWrite));
            if (bytesWrittenThisIteration == -1)
            {
                qDebug() << "Failed to write to file.";
                break;
            }
            bytesWritten += bytesWrittenThisIteration;
        }

        QDateTime endTime = QDateTime::currentDateTime();
        qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

        double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
        qDebug() << "Write Speed: " << writeSpeed << "MB/s";

        file.close();
        return writeSpeed;
    }
    else
    {
        qDebug() << "Failed to open the file for writing.";
        return -1;
    }
}

// 回调函数
void CALLBACK WriteCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    if (dwErrorCode != 0)
    {
        qDebug() << "WriteFileEx error: " << dwErrorCode;
        return;
    }

    // Your completion code here
}

double write_using_overlap_io()
{
    // 文件路径
    QString filePath = "write_using_overlap_io.dat";

    HANDLE hFile = CreateFile(
        filePath.toStdWString().c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_FLAG_OVERLAPPED,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        qDebug() << "Failed to open the file for writing.";
        return -1;
    }

    qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
    qint64 bytesWritten = 0;
    qint64 chunkSize = 1024 * 1024; // 1MB
    char buffer[chunkSize];
    QDateTime startTime = QDateTime::currentDateTime();
    DWORD bytesWrittenThisIteration;

    while (bytesWritten < totalBytesToWrite)
    {
        qint64 bytesToWrite = qMin(chunkSize, totalBytesToWrite - bytesWritten);
        OVERLAPPED overlapped;
        memset(&overlapped, 0, sizeof(OVERLAPPED));

        if (!WriteFile(hFile, buffer, static_cast<DWORD>(bytesToWrite), &bytesWrittenThisIteration, &overlapped))
        {
            DWORD dwError = GetLastError();
            if (dwError != ERROR_IO_PENDING)
            {
                qDebug() << "WriteFile error: " << dwError;
                break;
            }
            else{
                // Write operation is pending
                // Wait for the completion of the write operation
                if (!GetOverlappedResult(hFile, &overlapped, &bytesWrittenThisIteration, TRUE))
                {
                    qDebug() << "Write operation failed to complete.";
                    break;
                }
            }
        }

        bytesWritten += bytesWrittenThisIteration;
    }

    QDateTime endTime = QDateTime::currentDateTime();
    qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

    double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
    qDebug() << "Write Speed: " << writeSpeed << "MB/s";

    CloseHandle(hFile);
    return writeSpeed;
}

double write_using_buffer_manager()
{
    // 文件路径
    QString filePath = "write_using_buffer_manager.dat";

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
        qint64 bytesWritten = 0;
        qint64 chunkSize = 1024 * 1024; // 1MB
        QByteArray buffer(chunkSize, 'A');

        QDateTime startTime = QDateTime::currentDateTime();

        while (bytesWritten < totalBytesToWrite)
        {
            qint64 bytesToWrite = qMin(chunkSize, totalBytesToWrite - bytesWritten);
            out.writeRawData(buffer.data(), static_cast<int>(bytesToWrite));
            bytesWritten += bytesToWrite;
        }

        // 使用缓存管理器标记为可重用
        out.device()->seek(0);

        QDateTime endTime = QDateTime::currentDateTime();
        qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

        double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
        qDebug() << "Write Speed: " << writeSpeed << "MB/s";

        file.close();
        return writeSpeed;
    }
    else
    {
        qDebug() << "Failed to open the file for writing.";
        return -1;
    }
}

double write_using_multiple_threads()
{
    // 文件路径
    QString filePath = "write_using_multiple_threads.dat";
    qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB

    // 清空文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.resize(0);
        file.close();
    }

    int numThreads = 4; // 使用4个线程，可以根据需要进行调整

    if (file.open(QIODevice::WriteOnly))
    {
        file.resize(totalBytesToWrite); // Set the file size to the desired size
        QDateTime startTime = QDateTime::currentDateTime();

        qint64 chunkSize = totalBytesToWrite / numThreads;

        QList<WriteThread *> threads;
        for (int i = 0; i < numThreads; ++i)
        {
            qint64 startPosition = i * chunkSize;
            qint64 dataSize = (i == numThreads - 1) ? (totalBytesToWrite - startPosition) : chunkSize;
            threads.append(new WriteThread(file, dataSize, startPosition));
            threads[i]->start();
        }

        for (int i = 0; i < numThreads; ++i)
        {
            threads[i]->wait();
            delete threads[i];
        }

        QDateTime endTime = QDateTime::currentDateTime();
        qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

        double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
        qDebug() << "Total Write Speed: " << writeSpeed << "MB/s";
        return writeSpeed;
    }
    return -1;
}

double write_using_semaphore()
{
    // 文件路径
    QString filePath = "write_using_semaphore.dat";
    qint64 totalBytesToWrite = 4LL * 1024 * 1024 * 1024; // 4GB
    qint64 bufferSize = 1024 * 1024;                     // 1MB

    QByteArray buffer(bufferSize, 'A');
    QSemaphore semaphore(0); // Used for synchronization
    QMutex mutex;

    QDateTime startTime = QDateTime::currentDateTime();

    WriterThreadSemaphore writerThreadSemaphore(filePath, totalBytesToWrite, &buffer, &semaphore, &mutex);
    writerThreadSemaphore.start();

    // Wait for the writer thread to finish
    semaphore.acquire();

    QDateTime endTime = QDateTime::currentDateTime();
    qint64 elapsedMilliseconds = startTime.msecsTo(endTime);

    double writeSpeed = (static_cast<double>(totalBytesToWrite) / 1024 / 1024) / (elapsedMilliseconds / 1000.0);
    qDebug() << "Total Write Speed: " << writeSpeed << "MB/s";
    return writeSpeed;
}
