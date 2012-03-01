// to return a list of strings

#ifndef DPQ_MESSAGE_NAMES_H
#define DPQ_MESSAGE_NAMES_H

#include <iostream>
#include <string>

 
//**********************
//   Class Definition
//**********************
class DpqMessageNames
{
public:

DpqMessageNames();          // constructor - no return type

virtual ~DpqMessageNames();

//**************************************************
//
//  Description:    
//    this method returns the name of the messages
//
//  Public Member Functions:
//    - name
//
//  Protected Member Functions:
//
//  Private Member Functions:
//
//****************************************
virtual void name(const std::string& crMessageName);

virtual void output(std::ostream & ros) const;

private:

  string m_messageName;

}; //end class DpqMessageName

#endif // DPQ_MESSAGE_NAMES_H
