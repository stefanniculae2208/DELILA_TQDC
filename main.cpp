#include "../include/TQDC.hpp"




int main()
{

    TQDC caenDig;

    std::vector<EveData *> *data_loc = nullptr;



    caenDig.Open();

    caenDig.GetInfo();

    caenDig.Config();

    caenDig.QDCConfig();

    caenDig.AllocateMemory();


    caenDig.Start();

    caenDig.SendSWTrg(1);

    sleep(1);

    data_loc = caenDig.GetEvents();

    //std::cout<<"is "<<data_loc->at(0)->ChargeLong<<std::endl;

    caenDig.Stop();


    caenDig.FreeMemory();


    caenDig.Close();


    return 0;

}