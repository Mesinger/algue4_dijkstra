printf "\nTestscript for Dijkstra\n\n"

echo ----------------------------------------------------------------------------

#first test: simple trip without changing lines in both directions
printf "Test #1: Karlsplatz to Schwedenplatz\n"
printf "Expected Output: U1: 'Karlsplatz' 2 'Stephansplatz' 1 'Schwedenplatz' = 3min\n\n"
./dijkstra ../data/ubahn_sm.txt <<< "Karlsplatz Schwedenplatz"

echo ----------------------------------------------------------------------------

printf "Test #2: Schwedenplatz to Karlsplatz\n"
printf "Expected Output: U1: 'Schwedenplatz' 1 'Stephansplatz' 2 'Karlsplatz' = 3min\n\n"
./dijkstra ../data/ubahn_sm.txt <<< "Schwedenplatz Karlsplatz"

echo ----------------------------------------------------------------------------

#second test: one change, short trip both directions
printf "Test #3: Keplerplatz to Pilgramgasse\n"
printf "Expected Output: U4: 'Karlsplatz' 2 'Kettenbrueckengasse' 1 'Pilgramgasse' <<< U1: 'Karlsplatz' 2 'Taubstummengasse' 1 'Suedtirolerplatz' 2 'Keplerplatz' = 13min\n\n"
./dijkstra ../data/ubahn_sm.txt <<< "Keplerplatz Pilgramgasse"

echo ----------------------------------------------------------------------------

printf "Test #4: Pilgramgasse to Keplerplatz\n"
printf "Expected Output: U4: 'Karlsplatz' 2 'Kettenbrueckengasse' 1 'Pilgramgasse' >>> U1: 'Karlsplatz' 2 'Taubstummengasse' 1 'Suedtirolerplatz' 2 'Keplerplatz' = 13min\n\n"
./dijkstra ../data/ubahn_sm.txt <<< "Pilgramgasse Keplerplatz"

echo ----------------------------------------------------------------------------

#third test: same station
printf "Test #5: Westbahnhof to Westbahnhof\n"
printf "Expected Output: 0min\n\n"
./dijkstra ../data/ubahn_sm.txt <<< "Westbahnhof Westbahnhof"

echo ----------------------------------------------------------------------------

#fourth test: ring line
printf "Test #6: Floridsdorf to Siebenhirten\n"
printf "Expected Output: complete U6: 36min\n\n"
./dijkstra ../data/ubahn_sm.txt <<< "Floridsdorf Siebenhirten"

echo ----------------------------------------------------------------------------

printf "Test #6: Floridsdorf to Siebenhirten (loop, 10 min connection) \n"
printf "Expected Output: U6: 'Siebenhirten' 10 'Floridsdorf' = 10min\n\n"
./dijkstra ../data/ubahn_sm_u6loop.txt <<< "Floridsdorf Siebenhirten"