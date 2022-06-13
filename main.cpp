#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <ctime>
#include <sstream>
#include <chrono>
#include <numeric>
#include <array>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
#include <fstream>
#include <ctime>
#include <stdio.h>

using namespace std;
using namespace cv;

void IntercambiaDouble(double x, double y){
double aux = x;
x = y;
y = aux;
}

void OrdBurbuja (double v[], int util_v){
int izda, i;
bool cambio;
cambio = true;
for (izda=0; izda<util_v && cambio; izda++){
cambio=false;
for (i=util_v-1 ; i>izda ; i--){
if (v[i] < v[i-1]){
IntercambiaDouble(v[i], v[i-1]);
cambio=true;
}
}
}
}

namespace cpp_secrets{
///Runnable: A class which has a valid and public default ctor and a "run()" function.
///BenchmarkingTimer tests the "run()" function of Runnable
///num_run_cycles: It is the number of times run() needs to be run for a single test.
///One Runnable object is used for a single test.
///Note: if the run() function is statefull then it can only be run once for an object in order
///to get meaningful results.
///num_tests: It is the number of tests that need to be run.
    template <typename Runnable, int num_run_cycles = 1000000, int num_tests = 10>

        struct BenchmarkingTimer{
            /// runs the run() function of the Runnable object and captures timestamps around each test
            void run(){
                for(int i = 0; i < num_tests; i++){
                    Runnable runnable_object_{};
                    Timer t{intervals_[i].first, intervals_[i].second};
                    for(int i = 0; i < num_run_cycles; i++){
                        runnable_object_.run();
                    }
                }
            }

            ///utility function to print durations of all tests
            std::string durations() const{
                std::stringstream ss;
                int i{1};
                for(const auto& interval: intervals_){
                    ss << "Test-" << i++  << " duration = " << (interval.second - interval.first) * 0.001 << " ms" << std::endl;
                }
                return ss.str();
            }

            ///utility function to print average duration of all tests
            double average_duration(){
                auto duration_sum{0.0};
                for(const auto& interval: intervals_){
                    duration_sum += (interval.second - interval.first) * 0.001;
                }
                if (num_tests) return (duration_sum/num_tests);
                return 0;
            }

            private:
            std::array<std::pair<double, double>, num_tests> intervals_{};

            struct Timer{
                Timer(double& start, double& finish):finish_(finish) { start = now(); }
                ~Timer() { finish_ = now(); }

                private:
                double& finish_;
                double now(){
                         ///utility function to return current time in microseconds since epoch
                    return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
                }
            };

        };
}

///sample class which has a statefull run().
//run() function is stateful because it is not meaningful to sort a sorted array.
//that's why num_run_cycles = 1 in this case.
struct randomly_sorted{
    randomly_sorted(){
        srand(time(0));
        for(int i=0;i<1000000;i++){
            arr_.emplace_back(rand());
                  // making a vector filled with random elements
        }
    }

    void run(){
        sort(arr_.begin(), arr_.end(), std::less<int>());
    }
    private:
    std::vector<int>arr_;
};

unsigned t0,t1;
int main()
{

      Mat src, dst;
      float sum;

      /// Load an image
      src = imread("/home/debian/Documentos/Proyecto_Paralelo_FiltroG_Secuencial/pulmon.jpg", 0);
      t0=clock();
      t1 = clock();

      double time = (double(t1-t0)/CLOCKS_PER_SEC);
      cout <<"Tiempo de ejecución: " << time << endl;

      cpp_secrets::BenchmarkingTimer<randomly_sorted, 1, 10> test; // randomly_sorted structure run function is run 10 time and average output is given.
          test.run();
          std::cout << test.durations() << std::endl; // outputs the duration of every test.
          std::cout << "average duration = " << test.average_duration() << " ms" << std::endl;
      if( !src.data )
      { return -1; }

      clock_t tantes,tdespues;
        double tiempo;
        int puntuacion;

        //TEST DE CPU

        int cantidad=0;

        tantes = clock();

        cout<<"Pasando test de CPU..."<<endl;
        for(int i=0;i<1000000000;i++){
        cantidad = cantidad*2;
        cantidad = cantidad/2;
        cantidad++;
        cantidad--;
        }

        tdespues = clock();

        tiempo = static_cast<double>(tdespues - tantes)/CLOCKS_PER_SEC;
        cout<<"\nTiempo empleado: "<<tiempo<<" segundos"<<endl;

        puntuacion = 10000/tiempo;
        cout<<"\nPuntuacion obtenida: "<<puntuacion<<" puntos\n"<<endl;

        //TEST DE MEMORIA

        int componentes = 20000;
        double vector[componentes];

        //Llenamos el vector
        for(int i=0;i<componentes;i++){
        vector[i] = rand();
        }

        tantes = clock();

        cout<<"Pasando test de Memoria..."<<endl;
        //Aplicamos el algoritmo de ordenacion de la burbuja
        OrdBurbuja(vector, componentes);

        tdespues = clock();

        tiempo = static_cast<double>(tdespues - tantes)/CLOCKS_PER_SEC;
        cout<<"\nTiempo empleado: "<<tiempo<<" segundos"<<endl;

        puntuacion = 10000/tiempo;
        cout<<"\nPuntuacion obtenida: "<<puntuacion<<" puntos\n"<<endl;

      // define the kernel
      float Kernel[5][5] = {
                            {1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0},
                            {1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0},
                            {1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0},
                            {1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0},
                            {1/25.0, 1/25.0, 1/25.0, 1/25.0, 1/25.0}
                           };
         dst = src.clone();
        for(int y = 0; y < src.rows; y++)
            for(int x = 0; x < src.cols; x++)
                dst.at<uchar>(y,x) = 0.0;
        //convolution operation
        for(int y = 1; y < src.rows - 1; y++){
            for(int x = 1; x < src.cols - 1; x++){
                sum = 0.0;
                for(int k = -1; k <= 1;k++){
                    for(int j = -1; j <=1; j++){
                        sum = sum + Kernel[j+1][k+1]*src.at<uchar>(y - j, x - k);

                    }
                }
                dst.at<uchar>(y,x) = sum;



            }
        }


        namedWindow("Imagen Filtro Gaussiano");
        imshow("Imagen Filtro Gaussiano", dst);

        namedWindow("Imagen Original");
        imshow("Imagen Original", src);

      waitKey();


    return 0;
}
