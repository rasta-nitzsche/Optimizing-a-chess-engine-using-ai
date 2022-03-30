int minmax_ab( struct config *conf, int mode, int niv, int alpha, int beta, int largeur)
{
 	int n, i, score, score2;
 	struct config T[100];

   	if ( feuille(conf, &score) ) return score;                  // If we are at a leaf

	int quiescence=0;


	if ( niv == 0 ){
			   return quiescence_search(conf,mode,alpha,beta,largeur);
	}


   	if ( mode == MAX ) {

	   genererate_succ( conf, MAX, T, &n );

 	   if ( largeur != +INFINI ) {
	    	for (i=0; i<n; i++)
		     T[i].val = Estimate( &T[i] );

		qsort(T, n, sizeof(struct config), confcmp321);
		if ( largeur < n ) n = largeur;	  // we put a limitation of the depth
	   }

	   score = alpha;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax_ab( &T[i], MIN, niv-1, score, beta, largeur);
			if (score2 > score) score = score2;
			if (score >= beta) {
			// Beta
			nbBeta++;	// for alpha-beta pruning
   	      	return score;   
	    }
	   }
	}
	else  { // mode == MIN

	   genererate_succ( conf, MIN, T, &n );

 	   if ( largeur != +INFINI ) {
	    	for (i=0; i<n; i++)
		     T[i].val = Estimate( &T[i] );

		qsort(T, n, sizeof(struct config), confcmp123);
		if ( largeur < n ) n = largeur;	  
	   }

	   score = beta;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax_ab( &T[i], MAX, niv-1, alpha, score, largeur);
		if (score2 < score) score = score2;
		if (score <= alpha) {
			// Coupe Alpha
			nbAlpha++;	
   	      		return score;   
	    	}
	   }
	}

        if ( score == +INFINI ) score = +100;
        if ( score == -INFINI ) score = -100;
        
	return score;
} 