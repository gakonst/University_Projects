import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

import ithakimodem.Modem;

public class gpsMap {
	
	
	PrintWriter coords = null;
	
	public boolean Gps(Modem modem,String command, boolean gpsSent){
		int k;	
		virtualModem sender = new virtualModem();
		boolean gpsInitialized = false;
		
		try {
			coords = new PrintWriter(new FileOutputStream(new File("gps-coords.txt"),true));
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		for (;;){
			k=modem.read();
			if (k==-1 && (!gpsSent)) {
				if (!gpsInitialized){
					System.out.println("\n[==>] Initializing GPS");
					gpsInitialized=true;
				}
					sender.sendToModem(modem,command);
					gpsSent = true;
					
			}
			if (gpsSent){
				while ((k=modem.read()) !=-1){
					coords.write((char)k);
					//System.out.print((char)k);
					}
				
				coords.close();
				break;
			}
		}
		return gpsSent;
	}
		
	
}