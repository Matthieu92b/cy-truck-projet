#!/bin/bash

CSV_FILE="$1"
PROG_DIR="progc"
IMAGE_DIR="images"
TEMP_DIR="temp"
DEMO_DIR="demo"


# Fonction qui indique les touches à utiliser 
display_help() {
	echo "Pour éxécuter écrire $0 <nom_du_fichier_csv> [-h] [-d1] [-d2] [-l] [-t] [-s]"
	echo "Options:"
	echo "  -h        Affiche ce message d'aide"
	echo "  -d1       Effectue le traitement associé à l'option d1"
	echo "  -d2       Effectue le traitement associé à l'option d2"
	echo "  -l        Effectue le traitement associé à l'option l"
	echo "  -t        Effectue le traitement associé à l'option t"
	echo "  -s        Effectue le traitement associé à l'option s"
}


# Vérification du nombre d'arguments
if [ "$#" -lt 2 ]; then
	echo "Erreur : au moins deux arguments sont nécessaires."
	display_help
	exit 1
fi


# Vérification si l'option -h est présente
if [ "$2" = "-h" ]; then
	display_help
	exit 0
fi


# Vérification de la présence du fichier CSV d'entrée
if [ ! -f "$CSV_FILE" ]; then
	echo "Erreur : Le fichier CSV d'entrée '$CSV_FILE' n'existe pas."
	exit 1
fi


# Vérification et création du dossier temp
if [ ! -d "$TEMP_DIR" ]; then
	mkdir -p "$TEMP_DIR"
else
	# Vider le répertoire temp s'il existe déjà
	rm -f "$TEMP_DIR"/*
fi


# Vérification et création du dossier images
if [ ! -d "$IMAGE_DIR" ]; then
	mkdir -p "$IMAGE_DIR"
elif [ -z "$(find "$IMAGE_DIR" -maxdepth 0 -type d -empty 2>/dev/null)" ]; then
	#dirname permet de revenir au dossier precédent sans avoir besoin de le connaitre
	dossier_parent="$(dirname "$dossier_images")"
	TEMP_DIR="$dossier_parent/temp"
    	# deplacer les éléments du dossier image dans le dossier temps
    	mv "$IMAGE_DIR"/* "$TEMP_DIR"
fi



# Vérification et création du dossier démo
if [ ! -d "$DEMO_DIR" ]; then
	mkdir -p "$DEMO_DIR"
fi




# Traitement des options
for arg in "${@:2}"; do
    case "$arg" in
        -d1)
        # Temps de début
	start_time=$(date +%s)
	# Traitements pour l'option d1
	echo "Exécution traitement d1..."
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
	mv 'graphique_d1-r.png' "$DEMO_DIR/graphique_d1-r.png"
	mv 'graphique_d1.png' "$IMAGE_DIR/graphique_d1.png"
    	mv 'test_d1.csv' "$DEMO_DIR/test_d1.csv"
    	# Temps de fin
	end_time=$(date +%s)
	duration=$((end_time - start_time))
	# Affichage du temps d'exécution
	echo "Durée totale d'exécution : $duration secondes"
	;;
        -d2)
        # Temps de début
	start_time=$(date +%s)
	# Traitements pour l'option d2
	echo "Exécution traitement d2..."
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
	mv 'graphique_d2.png' "$IMAGE_DIR/graphique_d2.png"
    	mv 'test_d2.csv' "$DEMO_DIR/test_d2.csv"
    	mv 'graphique_d2-r.png' "$DEMO_DIR/graphique_d2-r.png"
   	# Temps de fin
	end_time=$(date +%s)
	duration=$((end_time - start_time))
	# Affichage du temps d'exécution
	echo "Durée totale d'exécution : $duration secondes"
	;;
        -l)
        # Temps de début
	start_time=$(date +%s)
	# Traitements pour l'option l
	echo "Exécution traitement l..."
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
	mv 'graphique_l.png' "$IMAGE_DIR/graphique_l.png"
    	mv 'test_l.csv' "$DEMO_DIR/test_l.csv"
    	# Temps de fin
	end_time=$(date +%s)
	duration=$((end_time - start_time))
	# Affichage du temps d'exécution
	echo "Durée totale d'exécution : $duration secondes"
	;;
        -t)
        # Temps de début
	start_time=$(date +%s)
	# Traitements pour l'option t
	echo "Exécution traitement t..."
        # Vérification de la présence de l'exécutable C
        if [ ! -d "$PROG_DIR" ]; then
		mkdir -p "$PROG_DIR"
    	elif [ -z "$(find "$PROG_DIR" -maxdepth 0 -type d -empty 2>/dev/null)" ]; then
    		dossier_parent3="$(dirname "$PROG_DIR")"
    		mv "$PROG_DIR"/* "$dossier_parent3"
   	fi
   	rm ./exec
	# Vérification de la présence de l'exécutable C
	if [ ! -f exec ]; then
    		echo "Compilation de l'exécutable C en cours..."
    		make all
    		if [ $? -ne 0 ]; then
        		echo "Erreur : La compilation a échoué. Veuillez vérifier les erreurs ci-dessus."
        		exit 1
    		fi
	fi
	# Exécution du programme avec redirection de la sortie
	./exec -t > fichier_t.txt
	if [ $? -ne 0 ]; then
    		echo "Erreur : L'exécution du programme a échoué."
    		exit 1
	fi
	head -n2 fichier_t.txt | tail -n1 | sed 's/->/->\n/g' | sed -e 's/\[//g' -e 's/\]//g' -e 's/->//g'  | head -n10  | sort -k1 > Données_graph_t.txt
	#traitement du fichier_t afin d'obtenir un fichier plus facile a travailler , a noter que la fonction sed permet de supprimer ou de remplacer un caractere
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
	mv 'graphique_t.png' "$IMAGE_DIR/graphique_t.png"
    	mv 'Données_graph_t.txt' "$DEMO_DIR/Données_graph_t.txt"
     	mv *.c *.h makefile "$PROG_DIR"
  	# Temps de fin
	end_time=$(date +%s)
	duration=$((end_time - start_time))
	# Affichage du temps d'exécution
	echo "Durée totale d'exécution : $duration secondes"
	;;
        -s)
        # Temps de début
	start_time=$(date +%s)
	# Traitements pour l'option s
	echo "Exécution traitement s..."
	if [ ! -d "$PROG_DIR" ]; then
		mkdir -p "$PROG_DIR"
     	elif [ -z "$(find "$PROG_DIR" -maxdepth 0 -type d -empty 2>/dev/null)" ]; then
    		dossier_parent2="$(dirname "$PROG_DIR")"
    		mv "$PROG_DIR"/* "$dossier_parent2"
   	fi
 	if [ ! -f ./exec ]; then
    		echo "Compilation de l'exécutable C en cours..."
    		make all 
    		if [ $? -ne 0 ]; then
        		echo "Erreur : La compilation a échoué. Veuillez vérifier les erreurs ci-dessus."
        		exit 1
    		fi
	fi
	# Exécution du programme avec redirection de la sortie
	./exec -s > fichier_s.txt
	if [ $? -ne 0 ]; then
    		echo "Erreur : L'exécution du programme a échoué."
    		exit 1
	fi
        tail -n52 fichier_s.txt | head -n50 |sed -E 's/\[|\]|->|,/ /g'  >Données_graph_s.txt
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
	mv 'graphique_s.png' "$IMAGE_DIR/graphique_s.png"
    	mv 'Données_graph_s.txt' "$DEMO_DIR/Données_graph_s.txt"
     	mv *.c *.h makefile "$PROG_DIR"
     	# Temps de fin
	end_time=$(date +%s)
	duration=$((end_time - start_time))
	# Affichage du temps d'exécution
	echo "Durée totale d'exécution : $duration secondes"
	#la on n'arrivait pas a remplir l'espace entre les courbes min et max (meme en creant une seule fonction), ducoup on a reussi a trouver une alternative en remplissant toute la partie en dessous de la courbe max (avec filledcurves above) puis en remplissant toute la partie en dessous de la courbe min en blanc (toujours avec la meme fonction) afin de se rapprocher le plus possible du graphique donnée. Le seul probleme de cette methode c'est que ça remplit tout ce qu'il y a en dessous en blanc meme les petites barres sur l'axe des x.
         ;;
        *)
         echo "Option inconnue : $arg"
         display_help
         exit 1
         ;;
    esac
done






