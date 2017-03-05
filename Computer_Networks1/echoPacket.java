import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.TimeUnit;

import ithakimodem.Modem;

public class echoPacket {
	boolean firstTime = false;	
	int runtime = 4*60; //RUNTIME FOR ECHO PACKETS IN SECONDS
	public void sendToModem(Modem modem, String command){
		byte[] commandBytes = command.getBytes();
		
		if (!firstTime) {
			System.out.print("[+] Command Issued: " + command);
			System.out.println("[+] Bytes passed: " + commandBytes);
			firstTime=true;
		}
		modem.write(commandBytes);
	}
	boolean echoInitialized = false;
	boolean writingSent = false;
	public boolean Echo(Modem modem, String command){
		PrintWriter echoTravel = null;
		PrintWriter echoes = null;

		try {
			 String fileName = new SimpleDateFormat("yyyy-MM-dd hh-mm-ss'.txt'").format(new Date());
			echoTravel= new PrintWriter(fileName,"UTF-8");
		} catch (FileNotFoundException | UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			 String fileName = new SimpleDateFormat("yyyy-MM-dd hh-mm-ss'-1.txt'").format(new Date());
			echoes= new PrintWriter(fileName,"UTF-8");
		} catch (FileNotFoundException | UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		int k;	 
		long tic = 0,toc =0;
		//virtualModem sender = new virtualModem();
		while((k=modem.read())!=-1){
			System.out.print((char)k);
		}
		if (k==-1) {
			if (!echoInitialized){
				System.out.println("[==>] Sending ECHO Packets for "+runtime+" seconds...");
				echoInitialized=true;
				}
			tic = System.currentTimeMillis();
			sendToModem(modem,command);
		}
		long stop=System.nanoTime()+TimeUnit.SECONDS.toNanos(runtime);
		String data ="";
		boolean reset = false;

		while((k=modem.read())!=-1){
			if (reset)data ="";
			//System.out.print((char)k);
			echoes.write((char)k);
			data+=(char)k;
			if (data.endsWith("PSTOP")){
				reset = true;
				toc = System.currentTimeMillis();
				float time = (float) (((float)(toc-tic)/1000));
				echoes.write(System.lineSeparator());
				echoes.write(Float.toString(time));
				echoes.write(System.lineSeparator());
				echoTravel.write(Float.toString(time));
				echoTravel.write(System.lineSeparator());
				tic = System.currentTimeMillis();
				modem.write(command.getBytes());
				if (stop<=System.nanoTime())break;
				}
			else{
				reset= false;}
		}
		
		//System.out.print("\nTime elapsed:"+time);
	echoes.close();
	echoTravel.close();
	return true;
		
		//System.out.println("[+] Time required: "+responseTime+" seconds | Speed = "+sender.getSpeed()+" kbps");
	}
}
