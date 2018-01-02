/**********************************************************************************************************************\

    DESCRIPTION: Handler to option a list of parameters, mods and server mods from the ArmA server

------------------------------------------------------------------------------------------------------------------------

    RESPONSIBILITY: Arkensor
    CREATION DATE:  30.12.2017

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018 Arkensor. All rights reserved!

\**********************************************************************************************************************/
#include "StartParameterHandler.hpp"

#ifdef _WINDOWS
    #include <Windows.h>
#endif

#include <algorithm>
#include <sstream>

namespace A3
{
namespace Extension
{
namespace StartParameter
{

CStartParameterHandler::CStartParameterHandler()
{
    LPSTR pParameter = GetCommandLineA();
    std::string strParameters( &pParameter[0] );

    while ( !strParameters.empty() )
    {
        //Clear unused spaces
        while( strParameters.front() == ' ' )
        {
            strParameters.erase( 0 , 1);
        }

        //Fetch the pParameter
        std::string strParameter;

        if( strParameters.front() == '"' )
        {
            size_t nEndPos = strParameters.find( '"', 1 );
            strParameter = strParameters.substr( 1 , ( nEndPos - 1 ) );
            strParameters.erase( 0, ( nEndPos + 1 ) );
        }
        else
        {
            size_t nEndPos = strParameters.find_first_of( " \"" );
            strParameter = strParameters.substr( 0 , nEndPos );
            strParameters.erase( 0, nEndPos );
        }

        //Make the parameters all the same format
        while( strParameter.front() == '-' )
        {
            strParameter.erase( 0 , 1);
        }

        //Find the key=value
        size_t nAssignmentPos = strParameter.find( '=' );

        //Add the key (and value if extits) to the map
        if( nAssignmentPos != std::string::npos )
        {
            std::string strKey = strParameter.substr( 0, nAssignmentPos );
            std::transform( strKey.begin(), strKey.end(), strKey.begin(), ::tolower );

            std::string strValue = strParameter.substr( nAssignmentPos + 1 );
            m_oStartParamters[ strKey ] = strValue;
        }
        else
        {
            m_oStartParamters[ strParameter ] = "";
        }
    }

    //Parse mods
    if( m_oStartParamters.count( "mod" ) > 0 )
    {
        std::stringstream oStream( m_oStartParamters.at( "mod" ) );
        std::string strMod;
        while ( getline( oStream, strMod, ';' ) )
        {
            m_oMods.push_back( strMod );
        }
    }

    //Parse servermods
    if( m_oStartParamters.count( "servermod" ) > 0 )
    {
        std::stringstream oStream( m_oStartParamters.at( "servermod" ) );
        std::string strMod;
        while ( getline( oStream, strMod, ';' ) )
        {
            m_oServerMods.push_back( strMod );
        }
    }
}

CStartParameterHandler::~CStartParameterHandler()
{
}

#ifndef _WINDOWS
LPSTR
CStartParameterHandler::GetCommandLineA()
{
	char *pstrPath = new char[ 4096 ];
	char *strCmdline = new char[ 4096 ];
	FILE *oFile;

	sprintf( pstrPath, "/proc/%ld/cmdline", (long)getpid() );
	oFile = fopen(pstrPath, "r");

	if (oFile)
	{
        size_t nRead = fread( strCmdline, sizeof(char), 4096, oFile );

        for( unsigned int nChar = 0; nChar < nRead; ++nChar ) //Todo - Do we need this?
        {
            if ( strCmdline[ nChar ] == 0 )
            {
                strCmdline[ nChar ] = ' ';
            }
        }

	    strCmdline[ nRead - 1 ] = '\0';

        fclose(oFile);
	}
	else
	{
        strCmdline[ 0 ] = '\0';
	}

    //Cleanup
    delete pstrPath;

	return strCmdline;
}
#endif

}; // end namespace StartParameter
}; // end namespace Extension
}; // end namespace A3