Individual::Individual(int clas) {
	for (int i = 0; i < 34; i++)
		genes[i].weight = double(i + i) / 100.;

	switch (clas) {
		case 1:
			// melanin incontinence
			genes[11].weight = 0.75;
			genes[11].op = EQUAL;
			genes[11].value = 0;

			// clubbing of the rete ridges
			genes[19].weight = 0.75;
			genes[19].op = GREATEREQUAL;
			genes[19].value = 1;

			break;

		case 2:
			// fibrosis of the papillary dermis
			genes[14].weight = 0.75;
			genes[14].op = EQUAL;
			genes[14].value = 0;

			// spongiosis
			genes[27].weight = 0.75;
			genes[27].op = DIFFERENT;
			genes[27].value = 0;

			// saw-tooth appearance of retes
			genes[28].weight = 0.75;
			genes[28].op = EQUAL;
			genes[28].value = 0;

			// perifollicular parakeratosis
			genes[30].weight = 0.75;
			genes[30].op = EQUAL;
			genes[30].value = 0;

			break;

		case 3:
			// spongiform pustule
			genes[22].weight = 0.75;
			genes[22].op = EQUAL;
			genes[22].value = 0;

			// band-like infiltrate
			genes[32].weight = 0.75;
			genes[32].op = GREATEREQUAL;
			genes[32].value = 2;

			break;

		case 4:
			// oral mucosal involvement
			genes[7].weight = 0.75;
			genes[7].op = EQUAL;
			genes[7].value = 0;

			// knee and elbow involvement
			genes[8].weight = 0.75;
			genes[8].op = EQUAL;
			genes[8].value = 0;

			// melanin incontinence
			genes[11].weight = 0.75;
			genes[11].op = GREATEREQUAL;
			genes[11].value = 0;

			// elongation of the rete ridges
			genes[20].weight = 0.75;
			genes[20].op = EQUAL;
			genes[20].value = 0;

			break;

		case 5:
			// oral mucosal involvement
			genes[7].weight = 0.75;
			genes[7].op = EQUAL;
			genes[7].value = 0;

			// fibrosis of the papillary dermis
			genes[14].weight = 0.75;
			genes[14].op = DIFFERENT;
			genes[14].value = 0;

			break;

		case 6:
			// hyperkeratosis
			genes[17].weight = 0.75;
			genes[17].op = GREATEREQUAL;
			genes[17].value = 0;

			// perifollicular parakeratosis
			genes[30].weight = 0.75;
			genes[30].op = DIFFERENT;
			genes[30].value = 0;

			break;

		default: break;
	}

	/*for (int i = 0; i < 34; i++)
		clog << "Gene " << i << ":\t" << genes[i].weight << '\t' << genes[i].op << '\t' << genes[i].value << endl;
	clog << endl;*/
}
