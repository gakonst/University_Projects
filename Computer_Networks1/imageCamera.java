import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import ithakimodem.Modem;

public class imageCamera {
	
	public static int sizeof(int s) { return 4; }

	public int writeImage(Modem modem, String fileName) throws IOException{
		//ERROR? Binary file is shit 
		ByteArrayOutputStream hex = new ByteArrayOutputStream();
		int k;
		String data="";
		char charRead;
		while ((k=modem.read()) != -1){
			charRead = (char)k;
			data+=charRead;
			}
		byte[] x= data.getBytes("ISO-8859-1");
		hex.write(x);

		if (k==-1){
			try {
				FileOutputStream img = new FileOutputStream(new File(fileName));
				System.out.println("[+] Writing from ByteArrayOutputStream to .JPEG file");
				hex.writeTo(img);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println("[!] k = -1 ... Exiting");
			}
		return k;
		
		}
	boolean imgInitialized = false;
	String kArray= null;

	public boolean Image(Modem modem, String command,boolean imgSent, String fileName) throws IOException{
		
		
		int k;	
		long tic = 0,toc =0;
		virtualModem sender = new virtualModem();
		/* Prints intro message until k=-1. After that it sends the 
		 * command and starts writing the binary characters to the file
		 * using the function writeImage. Before exiting, informs the user 
		 * about the time it took to write in correlation to the network speed. */
		for (;;){
		k=modem.read();
		if (k==-1 && !imgSent ) {
			if (!imgInitialized){
				System.out.println("[==>] Initializing Camera");
				imgInitialized=true;
				}
			//TIC
			tic = System.currentTimeMillis();
			//System.out.println("In class imageCamera.java: "+command);
			sender.sendToModem(modem,command);
			imgSent=true;
			}
		System.out.print((char)k);
		//Debug:
		kArray+=k;
		if (imgSent){
			System.out.println("[+] Writing to Image file... Please wait");
			//int count=0;
			writeImage(modem,fileName);
			/*(do{                           \\In this version writeImage doesn't have a while k!=-1 loop
				count++;
				k = writeImage(modem, img);
			}while(k!=-1);*/
			//System.out.println("[+] Finished writing after "+count+" iterations");
			toc = System.currentTimeMillis();
			float responseTime = ((float)(toc - tic))/1000;
			System.out.println("[+] Time required: "+responseTime+" seconds | Speed = "+sender.getSpeed()+" kbps\n");
			break;
			}
			//imgSent will be returned as true only after k is -1, meaning all characters have been printed
		 }
		return imgSent;

	}
}
