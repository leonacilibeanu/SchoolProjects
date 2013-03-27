import java.io.*;
import java.util.*;

public class DocumentParser
{
	private BufferedReader reader;
	private long pos;
	private long endPos;
	private HashMap<String, Integer> statistics;
	
	private boolean isLetter(char c)
	{

		char[] letters = {
				'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
		};
		
		for(int i = 0; i < letters.length; i++)
		{
			if(c == letters[i])
				return true;
		}
		
		return false;

/*
		//consideram ca facand parte din cuvant orice in afara de caracterele de spatiere
		if(c != '\r' && c != '\n' && c != ' ' && c != '\t' && c != ',' && c != '.' && c != (char)-1)
			return true;

		return false;*/
	}
	
	private boolean isLetter(int c)
	{
		return isLetter((char)c);
	}
	
	//citeste un caracter
	private int readChar() throws IOException
	{
		pos++;
		
		return reader.read();
	}
	
	//citeste un cuvant si il returneaza scris cu litere mici
	public String readWord() throws IOException
	{
		String result = "";
		
		int c = readChar();

		//sare peste caracterele dintre cuvinte
		while(!isLetter(c))
		{
			c = readChar();

			if(finished())
				break;
		}
		
		
		//citeste caracterele din cuvantul curent, citeste cuvantul intreg, chiar dc depaseste endpos
		while(isLetter(c))
		{
			result += (char)c;
			
			c = readChar();
		}
		
		return result.toLowerCase();
	}
	
	public HashMap<String, Integer> getStatistics()
	{
		return statistics;
	}
	
	//returneaza true daca a parcurs sectiunea alocata din document
	public boolean finished()
	{
		if(pos >= endPos)
			return true;
		
		return false;
	}
	
	public DocumentParser(String filename, long startPos, long endPos) throws IOException
	{
		this.endPos = endPos;
		
		reader = new BufferedReader(new FileReader(filename));
		
		pos = 0;
		FileInputStream fis;
		if(startPos > 0)
		{
			reader.skip(startPos - 1);
			pos = startPos - 1;
			
			//il marcam pentru a-l putea reseta
			reader.mark(0xFF);
			
			int c = readChar();
			
			if(isLetter(c))	//verifica daca suntem in mijlocul unui cuvant
			{
				//reset-ul este util atunci cand tocmai am citit ultima litera din cuvantul precedent
				reader.reset();
				
				String w = readWord();	//daca suntem in mijlocul unui cuvant ignora ce a mai ramas din el
			}
		}
		
		//calculeaza statisticile
		statistics = new HashMap<String, Integer>();
		
		while(!finished())
		{
			String word = readWord();
			
			if(word.length() == 0)
				continue;
			
			if(statistics.containsKey(word))
				statistics.put(word, statistics.get(word).intValue() + 1);
			else
				statistics.put(word, 1);
		}
	}
	
	public void close() throws Exception
	{
		reader.close();
	}
}
