#include "try.h"

using namespace std;

//std::ostream& os, const DpqMessageNames ::MessageName& messageName
//{


DpqMessageNames::DpqMessageNames()
{
}

DpqMessageNames::~DpqMessageNames()
{
}



//DpqMessageNames::DpqMessageNames(const string& crMessageName)
//      : m_name(crMessageName)
//{
//}


static std::string MessageName[] =
{
   "MC2_DPQ_APR_MSG",
   "MC2_DPQ_BSA_MSG",
   "MC2_DPQ_ER_MSG",
   "MC2_DPQ_SERVICE_COUNT_REPORT_MSG",
   "MC2_DPQ_SDL_MSG",
   "MC2_DPQ_SI_MSG",
   "MC2_DPQ_RESEND_MISSED_EPH_BLOCKS_MSG",
}


// get name

string DpqMessageNames::getName(int messageNumber)
{
   return MessageName[messageNumber];
}

 
int main()
{
    cout << "Starting program.." << endl;

    return 0;
}

