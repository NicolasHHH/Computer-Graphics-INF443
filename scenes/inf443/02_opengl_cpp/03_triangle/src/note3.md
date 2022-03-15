## VBO et VAO
### VBO - Vertex Buffer Object

- pointeurs:  
  - Identifiant --> données PAR SOMMET sur le GPU
- entrer dans le **vertex shader**
- GLSL : std::vector<float>

### VAO - Vertex Array Object

- les valeurs dans VBO sont **génériques** : 
  - peuvent désigner différents types d'attributs 
  - nécessaire d'indiquer la correspondances entre 
  les **données du buffer** et les **variables des shaders**
- stocke les relations entre l'organisation mémoire dans 
- le buffer et les variables du shader 
