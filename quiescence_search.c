int quiescence_search( struct config *conf, int mode, int alpha, int beta, int largeur)		
{
	int n, i, score, score2;
 	struct config T[100];

   	if ( feuille(conf, &score) )
		return score;

	if (stable(conf,mode) ){
		return Estimate( conf );
	}

   	if ( mode == MAX ) {

	   genereate_succ_quiescence( conf, MAX, T, &n );

 	   if ( largeur != +INFINI ) {
	    	for (i=0; i<n; i++)
		     T[i].val = Estimate( &T[i] );

			qsort(T, n, sizeof(struct config), confcmp321);
			if ( largeur < n ) n = largeur;	  
	   }

	   score = alpha;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = quiescence_search( &T[i], MIN, score, beta, largeur);
			if (score2 > score) score = score2;
			if (score >= beta) {
	
					nbBeta++;	
					return score;   
			}
	   }
	}
	else  { // mode == MIN

	   generate_succ_quiescence( conf, MIN, T, &n );

 	   if ( largeur != +INFINI ) {
	    	for (i=0; i<n; i++)
		    T[i].val = Estimate( &T[i] );

			qsort(T, n, sizeof(struct config), confcmp123);
			if ( largeur < n ) n = largeur;	  
	   }

	   score = beta;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = quiescence_search( &T[i], MAX, alpha, score, largeur);
			if (score2 < score) score = score2;
			if (score <= alpha) {
		
				nbAlpha++;	
   	      		return score;   
	    	}
	   }
	}

        if ( score == +INFINI ) score = +100;
        if ( score == -INFINI ) score = -100;

	return score;


} 


void generer_succ_quiescence( struct config *conf, int mode, struct config T[], int *n )	
{
	int i, j, k;
	struct config tab[100];
	int echec=0;
	int p=0;
	int pere=0;
	int fils=0;

	*n = 0;

	if ( mode == MAX ) {	
	   generate_succ( conf, MAX, tab, &p );

	   for (k=0; k<p; k++){
            fils = 0;
            pere = 0;
		   if(!dejaVisitee(&tab[k])){
			   	i = conf->xrB;
				j = conf->yrB;  // pos of king B
				if (caseMenaceePar( MIN, i, j, conf ))  echec = 1;

				for (int a=7; a>=0; a--){
					for (int b=7; b>=0; b--){
						if (tab[k].mat[a][b]<0 ){
							fils=fils+1 ;
						}
					}
		   		}
				for (int a=7; a>=0; a--){
					for (int b=7; b>=0; b--){
						if (conf->mat[a][b]<0 ){
							 pere=pere+1 ;
						}
					}
		   		}
				if ( (pere!=fils) || (echec) )	{
					copier(&tab[k],&T[*n]);
					*n = *n+1;
				}
	   		}
	   }
	}
	else { 				// mode == MIN
	   generer_succ( conf, MIN, tab, &p );
	   for (k=0; k<p; k++){
            fils = 0;
            pere = 0;
		   if(!dejaVisitee(&tab[k])){
			   	i = conf->xrN;
				j = conf->yrN;  // pos of king N
				if (caseMenaceePar( MAX, i, j, conf ))  echec = 1;

			for (int a=7; a>=0; a--){
				for (int b=7; b>=0; b--){
					if (tab[k].mat[a][b]>0 )	fils=fils+1 ;
					}
		   		}
				for (int a=7; a>=0; a--){
					for (int b=7; b>=0; b--){
						if (conf->mat[a][b]>0 ) pere=pere+1 ;
					}
		   		}
				if ( (pere!=fils) || (echec) )	{
					copier(&tab[k],&T[*n]);
					*n = *n+1;
				}
	   		}
	   }

	}
}


int stable(struct config *config, int mode){

	int i,j;

	for (i=7; i>=0; i--){
		for (j=7; j>=0; j--){
				if (mode == MAX && config->mat[i][j]<0 ){
					if (caseMenaceePar(MAX,i,j,config)) {
						return 0 ;
					}

				}
		else{
				if (mode == MIN && config->mat[i][j]>0 ) {
					if (caseMenaceePar(MIN,i,j,config)) return 0 ;
				}
			}
		}
	}

	if ( mode == MAX ) {
		i = config->xrB;
		j = config->yrB;
		if (caseMenaceePar( MIN, i, j, config )) {
			return 0;
		}
	}
	else { 				// mode == MIN
		i = config->xrN;
		j = config->yrN;  
		if (caseMenaceePar( MAX, i, j, config )) {
				return 0;
		}
	}
	return 1;
}
