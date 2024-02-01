#!/bin/bash

if [ "$1" = "-d2" ]; then
    awk -F ';' '{total[$6]+=$5} END { for(i in total) print total[i],i}' data.csv | sort -k1nr | head -n 10 > test.csv


    gnuplot -persist <<-EOF
        set terminal png size 1000,1000
        
        set output 'graph.png'
        set title 'Graphique des données'
        #set x2tics rotate
        #set ytics rotate
        #set ytics add("valeurs" )
        #set xlabel 'Valeurs'
        set ylabel 'Etiquettes'
        set style data histograms
        set boxwidth 0.8
        set yrange [0.5:*]
        set xrange [0:*]
        set style fill solid border -1
        

        plot 'test.csv' using 1:xticlabels(2) with boxes title 'Données'
EOF

    convert -rotate 90  graph.png Graphreverse.png
    rm graph.png  # Supprimer l'ancien fichier graph.png si nécessaire
   
fi

if [ "$1" = "-l" ]; then
awk -F ';' 'NR>1 {distance[$1]+=$5} END {for (i in distance) print i, distance[i] }' data.csv | sort -k2,2nr | head -n 10 >test2.csv

gnuplot -persist <<- EOF
	set terlinal png size 800,600
	set outpout 'graph.png'
	set title 'Graphique des données'
	set xlabel 'Valeurs'
	set ylabel ' Etiquettes'
	set style data histogram
	set style fill solid
	set boxwidth 0.5
	plot 'test2.csv' using 2:xticlabels(1) whith boxes title 'Données'
EOF
fi

if [ "$1" = "-t" ]; then
	
	make all
	./exec -t >tmp3.txt
	
	
fi

if [ "$1" = "-s" ];then 
	make all
	./exec -s > tmp4.txt
	
fi
