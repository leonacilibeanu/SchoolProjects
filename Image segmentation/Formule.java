import java.util.ArrayList;


public class Formule {

	/*
	 * @param Lista de tip Boolean corespunzatoare mastii. 
	 * @param Lista cu pixelii din imagine.
	 * @param Numarul de pixeli marcati cu valori mai mari ca zero in masca.
	 * 
	 * Calculeaza media valorilor pentru foreground sau background, in functie de 
	 * tipul parametrilor primiti.
	 * 
	 * @return Media valorilor.
	 */
	double calculateMiu(ArrayList<Boolean> mask, ArrayList<Integer> img, int nrMask){
		int i;
		double s,result;
		s=0;
		for (i=0;i<mask.size();i++) 
			if (mask.get(i)) 
				s+=img.get(i);
		
		result = s/Math.abs(nrMask);
		
		return result;
	}
	
	/*
	 * @param Lista de tip Boolean corespunzatoare mastii. 
	 * @param Lista cu pixelii din imagine.
	 * @param Numarul de pixeli marcati cu valori mai mari ca zero in masca.
	 * @param Media valorilor calculata pentru unul din medii (foreground/background).
	 * 
	 * Calculeaza deviatia standard pentru forground sau backgound, in functie de 
	 * tipul parametrilor primiti.
	 * 
	 * @return Deviatia standard.
	 */
	double calculateSigma(ArrayList<Boolean> mask, ArrayList<Integer> img, int nrMask, double miu){
		
		int i;
		double s;
		double result=0;
		s=0;
		
		for (i=0;i<mask.size();i++) 
			if (mask.get(i)) 
				s+=(miu-img.get(i))*(miu-img.get(i));
		
		result = Math.sqrt(s/Math.abs(nrMask));
				
		return result;
		
	}
	
	/*
	 * @param Pixelul din imagine.
	 * @param Media valorilor pentru foreground.
	 * @param Deviatia standard pentru foregound.
	 * @param Media valorilor pentru background.
	 * @param Deviatia standard pentru background.
	 * @param Parametrul functiei fu. Are valoarea 0 sau 1 in functie de tipul de calcul al capacitatii.
	 * 
	 * Calculeaza capacitatea pentru muchiile sursa-nod sau drena-nod.
	 * In primul caz, valoarea lui x este 1. In al doilea caz, valoarea lui x este 0.
	 * 
	 * @return Valoarea capacitatii muchiei.
	 */
	
	double calculateFu(int imgi,double miuf, double sigmaf, double miub, double sigmab, int x){
		double result = 0;
		
		result = x*(((0.5)*((imgi-miuf)/sigmaf)*((imgi-miuf)/sigmaf)) + 
				Math.log(Math.sqrt(2*Math.PI*(sigmaf*sigmaf)))) 
				+ (1-x)*(((0.5)*((imgi-miub)/sigmab)*((imgi-miub)/sigmab)) + 
						Math.log(Math.sqrt(2*Math.PI*(sigmab*sigmab))));
		
		result = Math.min(result, 10);
		
		
		return result;
		}
	
	/*
	 * @param Pixel din imagine.
	 * @param Pixel din imagine.
	 * @param Parametrul treshold din formula.
	 * 
	 * Calculeaza capacitatea muchiei dintre doua noduri primite drept parametru.
	 * 
	 * @return Valoarea capacitatii muchiei.
	 */
	
	int calculateFp(int imgi, int imgj, int treshold){
		
		if ((Math.abs(imgi-imgj) <= treshold)) 
			return 1;
		
		return 0;
	}
}
