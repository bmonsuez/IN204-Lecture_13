# Partie 2: Accélérer le chargement de la STL

Nous avons vu comment définir un module et comment utiliser un module. Maintenant, nous souhaitons pouvoir consommer les modules de la STL de manière automatique.

En fait, la plupart des modules de la STL peuvent être chargée de manière automatique comme un module précompilé; Nou considérons le fichier de code suivant que nous appellerons `hello_modular_world.cpp`:

```cpp
import <stdio>;

int main()
{
    std::cout << "Hello modular world!";
}
```

Nous remplaçons simplement l'instruction du préprocesseur par le mot-clé `import`. Et c'est tout, enfin presque tout.

## La compilation en faisant référence aux modules de la STL

A nouveau, Visual Studio supporte la référence aux modules out of the box. Il n'y a rien à faire, sauf à indiquer que l'on utilise C++20 ou plus.

Sinon, il faut ajouter quelques options supplémentaires. 

### Compilation sous CLang

Pour Clang, c'est assez simple, il faut indiquer que l'on fait référene aux modules prédérinis. Une fois cela fait, la compilation suit son cours:

```bash
clang++ -std=c++20 -stdlib=libc++ -fmodules -fbuiltin-module-map \
          -E hello_modular_world.cpp
```

### Compilation sous g++

Sous g++, nous devons avant tout générer les modules précompilés de la STL. Ceci sera obtenu par la commande suivante :

```bash
g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
```

qui indique que nous précompilons `iostream`.


Ensuite, il suffit de faire : 

```bash
g++ -std=c++20 -fmodules-ts hello_modular_world.cpp
```

et c'est terminé!

## Question n°1 : Compiler le code

Dans votre environnement, procéder à la compilation du code courant et vérifier que vous pouvez bien générer si nécessaire les fichiers précompilés.

## Question n°2 : Mesurer le temps de compilation

Comparer le temps de compilation en chargeant les entêtes ou en faisant référence aux modules. 

Sous Linux, vous pouvez utiliser la commande unix : `time`.

```bash
time clang++ -std=c++20 -stdlib=libc++ hello_world.cpp
```

Sous Windows, vous pouvez utiliser la commande suivante :

```cmd
powershell -Command Measure-Command { YOUR_COMMAND }
```

```powershell
```



