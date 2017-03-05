/*
Θέμα: 		   Εργασία 1 Δικτυών Υπολογιστών
Ονοματεπώνυμο: Κωνσταντόπουλος Γεώργιος
ΑΕΜ:		   8173
*/

/* 	Main class where the magic happens. Other classes are used to 
*  	help organize the project and make it more readable    */
import java.text.SimpleDateFormat;
import java.util.Date;

import ithakimodem.Modem;

public class virtualModem{
	public static void main(String[] param){
	 (new virtualModem()).rx();
	}

public void sendToModem(Modem modem, String command){
	
	byte[] commandBytes = command.getBytes();
		
	System.out.print("[+] Command Issued: "+command);
	System.out.println("[+] Bytes passed: "+commandBytes);
	System.out.print("\n");
	modem.write(commandBytes);
	
	}
public int getSpeed(){return 80000;}

public void rx() {
	 /*  Request Codes Pool
	  *  from http://ithaki.eng.auth.gr/netlab/ 
	  */
	 imageCamera camera = new imageCamera();
	 gpsMap gps = new gpsMap();
	 echoPacket echo = new echoPacket();
	 Ack ack = new Ack();
	 Coords coordsFetch = new Coords();
	 Modem modem;
	 modem = new Modem();
	 modem.setSpeed(getSpeed());
	 modem.setTimeout(2000);
	 modem.open("ithaki");
	 
	 final String echo_command = "E9895\r";
	 final String img_command = "M6213\r";
	 final String gps_command = "P4737=1000099\r";
	 final String noise_command = "G6041\r";
	 String gpsmapCoords="";
	 String gmaps_command = gps_command.substring(0, 5);
	 final String ack_command = "Q6532\r";
	 
	 boolean debugCamera = false;
	 boolean debugNoise = false;
	 boolean debugMaps = false;
	 boolean debugGps = false;
	 boolean debugCoords = false;
	 boolean debugEcho = true;
	 boolean debugAck = true;
	 
	 boolean imgSent = false;
	 boolean gpsSent = false;
	 boolean echoSent = false;
	 boolean ackInfo = false;
	 
	 if (debugCamera) {
		//Camera image request
		 try {
			 String fileName = new SimpleDateFormat("yyyy-MM-dd hh-mm-ss'.jpeg'").format(new Date());

				imgSent = camera.Image(modem, img_command, imgSent, fileName);
				if (imgSent){
					System.out.println("cameraNoNoise.jpeg file successfully written!\n");
				}
		}catch (Exception x) {
				x.printStackTrace();
			}
	 }
	 
	 //Camera image request	 
	 imgSent = false;
	 if (debugNoise) {
			try {
				 String fileName = new SimpleDateFormat("yyyy-MM-dd hh-mm-ss'.jpeg'").format(new Date());

				imgSent = camera.Image(modem, noise_command, imgSent, fileName);
				if (imgSent){
					System.out.println("cameraNoise.jpeg file successfully written!\n");
				}
			}catch (Exception x) {
				x.printStackTrace();
				}
		}
	 
	 //Gps Request
	 if (debugGps){
		 gpsSent = gps.Gps(modem, gps_command, gpsSent);
		 String gps_command1 = "P4737R=1010099\r";
		 gpsSent = false;
		 gpsSent = gps.Gps(modem, gps_command1, gpsSent);
		 gps_command1 = "P4737R=1020099\r";
		 gpsSent = false;
		 gpsSent = gps.Gps(modem, gps_command1, gpsSent);
		 gps_command1 = "P4737R=1030099\r";
		 gpsSent = false;
		 gpsSent = gps.Gps(modem, gps_command1, gpsSent);
		 gps_command1 = "P4737R=1040099\r";
		 gpsSent = false;
		 gpsSent = gps.Gps(modem, gps_command1, gpsSent);		 
	 	}
	 if (gpsSent)System.out.println("GPS file with Coordinates succesffully written!");
	 
	 //Echo request. 	
	 if (debugEcho)echoSent = echo.Echo(modem, echo_command);
	 if (echoSent)System.out.print("File with response times between each echo successfully written. "
			+ "Import the .txt file's data to Matlab/Excel like software to convert to graph.\n");

	 //Modify gmaps_command to include 4 traces. 
	 if (debugCoords){
		 gpsmapCoords=coordsFetch.coords();
		 gmaps_command = gmaps_command+gpsmapCoords+"\r";
	 }
	 else{
		 gmaps_command = gmaps_command+"\r";
	 }
	 
	 //Set imgSent to false from previous calls, use gmaps_command to get image with 4 traces on it.
	 imgSent = false;
	 if (debugMaps) {
		//Camera image request
		try {
;			//System.out.println("[*] DEBUG: PRINTING GMAPS: "+gmaps_command);
String fileName = new SimpleDateFormat("yyyy-MM-dd hh-mm-ss'.jpeg'").format(new Date());

			imgSent = camera.Image(modem, gmaps_command, imgSent, fileName);
			if (imgSent){
				System.out.println("[+] gMaps.jpeg file successfully written!\n");
				}
			}catch (Exception x) {
			x.printStackTrace();
		}
	 } 
	 //ackInfo = True if the xor matches the decimal number. If ackInfo is false, send NACK command.
	 if (debugAck)ackInfo = ack.ackError(modem,ack_command,ackInfo); 

	 
	 
	 
	//Close connection
	 System.out.println("Done! Exiting...");
	 modem.close();
 }
}