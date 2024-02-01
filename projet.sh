#!/bin/bash


if [ "$1" = "-d1" ]; then
	grep ";1;" data.csv | sort -n | cut -d ';' -f6 | sort -t ' ' -k2 | uniq -c | sort -k1,1nr -k3,3 -k2,2 |head -10 >test_d1.csv

#le grep permet de trouver toutes les premières étapes d'un trajet
gnuplot -persist <<- EOF
	set terminal png size 1000,1000
	set output 'graphique_d1-r.png'
	set title ' '
	set xlabel 'DRIVER NAMES'
	set ylabel 'Option -d1 : Nb routes = f(Driver) '
	set y2label 'NB ROUTES' 
	set style data histograms
	set boxwidth 0.8
	set xrange[0:*]
	set y2range[0:*]
	set yrange[0:*]
	set style fill solid border -1
	set xtic rotate 
	set y2tics rotate       
	set xlabel rotate by 180
	set y2tic mirror	
	unset ytic 
	set offsets 0.8, 0, 0, 0
	concat_name(col) = sprintf("%s %s", stringcolumn(col), stringcolumn(col + 1))
	plot 'test_d1.csv' using 1:xtic(concat_name(2)) with boxes notitle lc rgb '#00FF00' axes x1y2
EOF
#on fait notre graphique a l'envers pour pouvoir faire une rota et donc obtenir un graph horizontal. De plus , on a crée une fonction qui permet de concatener une chaine de caractère afin d'afficher le prenom et le nom
touch graph.png

convert 'graphique_d1-r.png' -rotate 90 'graphique_d1.png' 
fi 




if [ "$1" = "-d2" ]; then
   awk -F ';' '{total[$6]+=$5} END { for(i in total) print total[i],i}' data.csv | sort -k1nr | head -n 10 > test_d2.csv


gnuplot -persist <<- EOF
	set terminal png size 1000,1000
	set output 'graphique_d2-r.png'
	set title ' '
	set xlabel 'DRIVER NAMES'
	set ylabel 'Option -d2 : Distance = f(Driver) '
	set y2label 'DISTANCE (Km)' 
	set style data histograms
	set boxwidth 0.8
	set xrange[0:*]
	set y2range[0:*]
	set yrange[0:*]
	set style fill solid border -1
	set xtic rotate 
	set y2tics rotate       
	set xlabel rotate by 180
	set y2tic mirror	
	unset ytic 
	set offsets 0.8, 0, 0, 0
	concat_name(col) = sprintf("%s %s", stringcolumn(col), stringcolumn(col + 1))
	plot 'test_d2.csv' using 1:xtic(concat_name(2)) with boxes notitle lc rgb '#00FF00' axes x1y2
EOF
#meme chose que le graph d1
touch graph.png

convert 'graphique_d2-r.png' -rotate 90 'graphique_d2.png' 
   
fi

if [ "$1" = "-l" ]; then
awk -F ';' 'NR>1 {distance[$1]+=$5} END {for (i in distance) print i, distance[i] }' data.csv | sort -k2,2nr | head -n 10 | sort -k1nr >test_l.csv
gnuplot -persist <<- EOF
	set terminal png size 1000,1000
	set output 'graphique_l.png'
	set title 'Option -l : Distance = f(Route)'
	set xlabel 'ROUTE ID'
	set ylabel ' DISTANCE (km)'
	set style data histogram
	set style fill transparent solid 0.5
	set style fill solid
	set boxwidth 0.8
	set xrange[0:*]
	set yrange[0:*]
	set offsets 0.8, 0, 0, 0
	plot 'test_l.csv' using 2:xticlabels(1) with boxes notitle lc rgb '#00FF00'
EOF

#là c'est juste un histogramme normal , sans complexité nouvelle
fi

if [ "$1" = "-t" ]; then
	
	make all
	./exec -t >fichier_t.txt

head -n2 fichier_t.txt | tail -n1 | sed 's/->/->\n/g' | sed -e 's/\[//g' -e 's/\]//g' -e 's/->//g'  | head -n9  | sort -k1 > Données_graph_t.txt
#traiteùent du fichier_t afin d'obtenir un fichier plus facile a travailler , a noter que la fonction sed permet de supprimer ou de remplacer un caractere

gnuplot -persist <<- 'EOF'
    set terminal pngcairo enhanced font 'arial,12' size 1200, 1000
    set output 'graphique_t.png'
    set title 'Option -t : Nb routes = f(Towns)'
    set key at screen 0.95,0.93
    set style data histogram
    set style histogram clustered gap 5
    set style fill transparent solid 1
    set boxwidth 3
    set style fill transparent solid 0.5
    #permet de faire en sorte que les bordures ne soit plus solides afin de les superposer, de def l'espace entre les boites et comment elles rendent sur le graphique
    
    set xtics rotate by -45
    set ytics nomirror

    set xlabel 'TOWN NAMES'
    set ylabel 'NB ROUTES'

    datafile = 'Données_graph_t.txt'
    set datafile separator ','  
     plot datafile using 2:xtic(1) title 'Total routes' lc rgb '#00FF00', \
         datafile using 3:xtic(1) title 'First town' lc rgb '#008000'
  
EOF

fi

if [ "$1" = "-s" ];then 
	make all
	./exec -s > fichier_s.txt
	tail -n52 fichier_s.txt | head -n50 |sed -E 's/\[|\]|->|,/ /g'  >Donnees_graph_s.txt
#traiteùent du fichier_s afin d'obtenir un fichier plus facile a travailler , a noter que la fonction sed permet de supprimer ou de remplacer un caractere

gnuplot -persist <<- 'EOF'
    set terminal pngcairo enhanced font 'arial,12' size 1300,1000
    set output 'graphique_s.png'
    set title 'Option -s : Distance = f(Route)'
    set style data histograms
    set yrange[0:*]
    set xrange[0:*]
    set key off

    set xtics rotate by -45
    set ytics nomirror
    set style fill solid border -1
    set xlabel 'ROUTE ID'
    set ylabel 'DISTANCE (Km)'
    set y2label ' '

    datafile = 'Donnees_graph_s.txt'
plot datafile using 4:xtic(1) title 'Max' with filledcurves above lc rgb '#90EE90', \
     datafile using 2:xtic(1) title 'Min' with filledcurves above lc rgb '#FFFFFF', \
     datafile using 3:xtic(1) title 'Moyenne' with lines lc rgb '#000000'

EOF

#la on n'arrivait pas a remplir l'espace entre les courbes min et max (meme en creant une seule fonction), ducoup on a reussi a trouver une alternative en remplissant toute la partie en dessous de la courbe max (avec filledcurves above) puis en remplissant toute la partie en dessous de la courbe min en blanc (toujours avec la meme fonction) afin de se rapprocher le plus possible du graphique donnée. Le seul probleme de cette methode c'est que ça remplit tout ce qu'il y a en dessous en blanc meme les petites barres sur l'axe des x.

fi
