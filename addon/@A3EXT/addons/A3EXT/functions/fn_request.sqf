/**********************************************************************************************************************\

    DESCRIPTION: A3EXT - an extension framework for ArmA 3

------------------------------------------------------------------------------------------------------------------------

    RESPONSIBILITY: Arkensor
    CREATION DATE:  01.01.2018

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018 Arkensor. All rights reserved!

\**********************************************************************************************************************/

params
[
    [ "_data", "", [ "", 0 ] ],
    [ "_return", 1, [ 0 ] ]
];

private _ticketID = [ _data, _return ] call A3EXT_fnc_enqueue;

if ( _return isEqualTo 1 && ( !( _ticketID isEqualTo -1 ) ) ) exitWith
{
    private _result = [ _ticketID ] call A3EXT_fnc_dequeue;
    _result;
};