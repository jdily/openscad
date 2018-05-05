#pragma once

#include <QtCore>
#include <Eigen/Dense>
#include <iostream>
// #include "UtilityGlobal.h"


class LFD {

public:
    static int lfd_feat(int node_count, QString filename, QString basepath) {
        std::cout << "lfd feat " << node_count << std::endl;
        QString csg_obj_basepath = QString("%1/csginfo/%2").arg(basepath).arg(filename);
        // QString LFD_path = QString("./external/LFD");
        // QString LFD_path = QDir::currentPath() + "/external/LFD";
        QString LFD_path = QString("/home/ichao/projects/ddCAD/lib/openscad/external/LFD");
        QString LFD_list_path = LFD_path + "/list.txt";
        QFile file(LFD_list_path);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QTextStream input(&file);
        for (int i = 1; i <= node_count; i++) {
            QString obj_path = QString("%1/%2").arg(csg_obj_basepath).arg(i);
            std::cout << obj_path.toStdString() << std::endl;
            input << obj_path << endl;
        }
        file.close();
        // QString LFD_exe = LFD_path + "/LFD";
    }


    static int calc(const QString filename, const QVector<QString> ObjList, const QVector<QString> NameList, Eigen::MatrixXd &DistM) {
        // QString filepath = QDir::currentPath() + "/../lib/LFD";
        // QString ListPath = filepath + "/List.txt";
        // QFile file(ListPath);
        // file.open(QIODevice::ReadWrite | QIODevice::Text);
        // QTextStream input(&file);

        // // create the cache folder if not exists
        // QString cacheFolder = filepath + "/LFD/Cache/";
        // QDir dir(cacheFolder);
        // if (!dir.exists()) {
        //     dir.mkpath(cacheFolder);
        // }
        // input << "L " << filename << endl;
        // input << "N " << ObjList.size() << endl;
        // input << "D " << cacheFolder << endl;
        // for (int i = 0; i < ObjList.size(); i++) {
        //     input << ObjList[i] << endl;
        //     input << NameList[i] << endl;
        // }
        // file.close();

        // QString commandq = filepath;
        // LPCWSTR command_l = (const wchar_t*)commandq.utf16();
        // wchar_t buf[1000];
        // GetCurrentDirectory(1000,buf);
        // SetCurrentDirectory(command_l);
        // system("LFD.exe");
        // SetCurrentDirectory(buf);
        
        // ///////////////////////////////////////////////////////////////////
        // DistM = Eigen::MatrixXd::Zero(ObjList.size(),ObjList.size());
        // QFile fileDist(filename+"_dist_mat.txt");
        // fileDist.open(QIODevice::ReadWrite | QIODevice::Text);
        // QTextStream output(&fileDist);

        // QString line = output.readLine();
        // line = output.readLine();
        // line = output.readLine();
        // int count = 0;
        // int linewidth_pre = 0;
        // int linewidth = ObjList.size() - 1;
        // int linehight = 0;
        // double maxDist = 0; 
        // while(!line.isNull())
        // {
        //     double dist = line.toDouble();
        //     if(dist > maxDist)
        //         maxDist = dist;
        //     int indexX,indexY;
        //     if(count < linewidth)
        //     {
        //         indexX = linehight;
        //         indexY = count - linewidth_pre + linehight + 1;
        //     }
        //     else
        //     {
        //         int tmp = linewidth - linewidth_pre;
        //         linewidth_pre = linewidth;
        //         linewidth += tmp - 1;
        //         linehight++;
        //         indexX = linehight;
        //         indexY = count - linewidth_pre + linehight + 1;
        //     }

        //     DistM(indexX,indexY) = dist;
        //     DistM(indexY,indexX) = dist;
        //     count ++;
        //     line = output.readLine();
        // }
        // DistM = DistM/maxDist;
        // matrixToFile(DistM, filename+".csv");
        return 1; 
    }

    // static inline void matrixToFile(const Eigen::MatrixXd & M, QString filename)
    // {
    //     QFile file( filename );
    //     if(!file.open(QFile::WriteOnly | QFile::Text)) return;
    //     QTextStream out(&file);
    //     for(unsigned int i = 0; i < M.rows(); i++)
    //     {
    //         QStringList row;
    //         for(unsigned int j = 0; j < M.cols(); j++)
    //             row << QString::number(M(i,j));
    //         out << (row.join(",") + "\n");
    //     }
    // }
};

// static int calc(const QString filename, const QVector<QString> ObjList, const QVector<QString> NameList, Eigen::MatrixXd &DistM)
// {
//     QString filepath = QDir::currentPath() + "/../UtilityLib";

//     QString ListPath = filepath + "/LFD/List.txt";
//     QFile file(ListPath);
//     file.open(QIODevice::ReadWrite | QIODevice::Text);
//     QTextStream input(&file);

//     // create the cache folder if not exists
//     QString cacheFolder = filepath + "/LFD/Cache/";
//     QDir dir(cacheFolder);
//     if (!dir.exists())
//     {
//         dir.mkpath(cacheFolder);
//     }

//     input << "L " << filename << endl;
//     input << "N " << ObjList.size() << endl;
//     input << "D " << cacheFolder << endl;
//     for (int i = 0; i < ObjList.size(); i++)
//     {
//         input << ObjList[i] << endl;
//         input << NameList[i] << endl;
//     }

//     file.close();

//     QString commandq = filepath + "/LFD";
//     LPCWSTR command_l = (const wchar_t*) commandq.utf16();
//     wchar_t buf[1000];
//     GetCurrentDirectory(1000,buf);
//     SetCurrentDirectory(command_l);
//     system("LFD.exe");
//     SetCurrentDirectory(buf);

//     ///////////////////////////////////////////////////////////////////
//     DistM = Eigen::MatrixXd::Zero(ObjList.size(),ObjList.size());
//     QFile fileDist(filename+"_dist_mat.txt");
//     fileDist.open(QIODevice::ReadWrite | QIODevice::Text);
//     QTextStream output(&fileDist);

//     QString line = output.readLine();
//     line = output.readLine();
//     line = output.readLine();
//     int count = 0;
//     int linewidth_pre = 0;
//     int linewidth = ObjList.size() - 1;
//     int linehight = 0;
//     double maxDist = 0; 
//     while(!line.isNull())
//     {
//         double dist = line.toDouble();
//         if(dist > maxDist)
//             maxDist = dist;
//         int indexX,indexY;
//         if(count < linewidth)
//         {
//             indexX = linehight;
//             indexY = count - linewidth_pre + linehight + 1;
//         }
//         else
//         {
//             int tmp = linewidth - linewidth_pre;
//             linewidth_pre = linewidth;
//             linewidth += tmp - 1;
//             linehight++;
//             indexX = linehight;
//             indexY = count - linewidth_pre + linehight + 1;
//         }

//         DistM(indexX,indexY) = dist;
//         DistM(indexY,indexX) = dist;
//         count ++;
//         line = output.readLine();
//     }
//     DistM = DistM/maxDist;
//     matrixToFile(DistM, filename+".csv");

//     return 1;
// }