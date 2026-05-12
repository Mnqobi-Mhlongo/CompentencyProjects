SELECT AVG(songs.energy)
FROM artists
JOIN songs
ON artists.id = songs.artist_id
WHERE artists.name = 'Drake';
