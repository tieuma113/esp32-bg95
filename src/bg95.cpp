#include "bg95.h"

// Return the IP that network assigns to the NB-IoT module
String Bg95::getIP() 
{
	return ip;
}

// Return the IP that network assigns to the NB-IoT module
String Bg95::getIMEI() 
{
	return imei;
}

Bg95::Bg95(){
    ip = "";
    imei = "";
    socket = 0;
}

bool Bg95::begin(){
    BG95.begin(115200,SERIAL_8N1,16,17);
    DEBUG.begin(9600);

    while (!BG95);

    return setup();
}

bool Bg95::setup(){
	bool flag;
	flag = isAlive();
	while(!flag){
		flag = isAlive();
	}
	if (flag){
		flag = confNbiotConnection();
	}
	if (flag){
		flag = networkRegistered();
	}
	if (flag){
		flag = askForIp();
	}
	if (flag){
		flag = askForImei();
	}
	return flag;
}

bool Bg95::networkRegistered(){
	bool flag = true;					// Flag for checking correct working of AT commands

	if (flag) 
	{
		String atCommand = "AT+COPS=0,0,\"\",9";// Building AT Command

		sendAT (atCommand);				// Send AT command
		flag = checkRespForOpSelection ();
	}

	if (flag)
	{
		sendAT ("AT+CEREG?");			// Send AT command
		flag = checkRespForReg ();
	}
	

	return flag;
}

bool Bg95::checkRespForReg (){
	int timeOut = 300;					// From BG96 Quectel AT Commands Manual
	String response;
	bool ok = false;
	unsigned long startTime = millis();	// Take time at start for time out

	// Check every char string received for CEREG message
	while (!ok && ((millis()-startTime) <= (unsigned long) timeOut) ) {
		response += receiveAT();
		ok = (response.indexOf("+CEREG: 0,1") >= 0);
	}

	return ok;
}

bool Bg95::checkRespForOpSelection(){
	int timeOut = 180000;
	String response;
	bool ok = false;
	bool error = false;
	unsigned long startTime = millis();	// Take time at start for time out

	// Check every char string received for "OK" response
	while ( !error && !ok && ((millis()-startTime) <= (unsigned long) timeOut) ) {
		response += receiveAT();
		ok = (response.indexOf("\r\nOK\r\n") >= 0);	// Check if "OK" is in the response
		error = (response.indexOf("\r\n+CME ERROR:") >= 0);	// Check if "ERROR" is in response
	}

	return ok;
}

bool Bg95::confNbiotConnection(){
	bool flag = true;					// Flag for checking correct working of AT commands

	if (flag) {
		sendAT("AT+CGDCONT=1,\"IP\",\"\"");
		flag = checkOK (500);
	}

	if (flag) {
		sendAT("AT+CFUN=1");
		flag = checkOK (15000);
	}

	return flag;
}

bool Bg95::askForIp(){
    String receivedIp = "";
    sendAT("AT+CGPADDR");
    receivedIp = checkIp();
    if (receivedIp.length() > 0 ){
        ip = receivedIp;
        return true;
    }else {
        return false;
    }
}

bool Bg95::isAlive() 
{
	sendAT ("AT");						// Send AT command
	return checkOK ( 1000 );		// Check the response during time out
}

String Bg95::checkIp(){
    int timeout = 300;
    String response;
    String receivedIp = ""; 
    bool ok = false;
    unsigned long startTime = millis();

    while ( !ok && ((millis()-startTime) <= (unsigned long) timeout) ) {
		response += receiveAT();
		if (response.indexOf("\r\nOK\r\n") >= 0) {	// Check all the response is received
			if (response.indexOf("\r\n+CGPADDR:") >= 0) {
				receivedIp = response.substring((1+response.indexOf(",")));
				receivedIp = receivedIp.substring(0,receivedIp.indexOf("\r\n"));
			}
		}
	}

	return receivedIp;
}

// Saves the IMEI of SIM card
bool Bg95::askForImei ( ) 
{
	String receivedImei;

	sendAT("AT+CGSN");					// Send AT command

	receivedImei = checkImei ();	// Receive and parse the IMEI

	if ( receivedImei.length() > 0 ) {
		imei = receivedImei;
		return true;
	} else {
		return false;
	}
}

String Bg95::checkImei () 
{
	int timeOut=300;
	String response;
	String receivedImei = "";
	bool ok = false;
	unsigned long startTime = millis();	// Take time at start for time out

	while ( !ok && ((millis()-startTime) <= (unsigned long) timeOut) ) {
		response += receiveAT();
		if (response.indexOf("\r\nOK\r\n") >= 0) {	// Check all the response is received
			// The firsts chars received are \r\n
			receivedImei = response.substring(2);
			receivedImei = receivedImei.substring(0,receivedImei.indexOf("\r\n")); 
			ok = true;
		}
	}

	return receivedImei;
}

String Bg95::receiveAT(){
    	String received;

	// Save data received from BG96 module
	while (BG95.available()) {
		received += (char)BG95.read ();
	}
#ifdef DEBUGMODE
	printAT (received);					// DEBUG: Print response from BG96 module
#endif

	return received;	
}

void Bg95::printAT ( String text ) 
{
	if (text.length() > 0) {			// If string is not empty ...
		DEBUG.print(text);				// Send by debug serial port the string
	}
}