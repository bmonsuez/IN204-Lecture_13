# Partie 1 : Introduction aux modules


## Présentation simplifiée des modules en C++

Nous souhaitons écrire notre premier module qui exporte une fonction.

```cpp
export module math

export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}
```

Ce module définit un module dénomé `math`:

```cpp
module math
```

Il définit une fonction qui est visible en dehors du module :


```cpp
export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}
```

Cette fonction peut donc être utilisée par un code qui fait référence
à ce module.

Ainsi, le code suivant :


```cpp
import math;

int main()
{
    fact(20);
    return 0;
}
```

indique que le module `math` doit être importé et de fait il appelle la fonction 
`fact(int n)` définie dans ce module pour la valeur `20`.

## Les différences entre les modules et les includes

En fait, dans l'approche standard de C/C++, nous définissons une unité de compilation comme un ensemble d'un ou plusieurs fichiers :

* un fichier `.cpp` qui contient le code devant être compilé
* un fichier `.hpp` qui contient les déclaations devant être exportées, voire éventuellement un fichier `.template` qui contient les déclarations ainsi que le code des classes et fonctions templatées, un fichier `.inline` qui contient le code des classes et fonctions qui peuvent être inlinées.

La pratique fait que de plus en plus de code migre du fichier `.cpp` vers les fichiers `.hpp`. Souvent nous nous retrouvons avec des fichiers `.cpp` qui ne contiennent plus beaucoup de code ou qui sont vides. De plus en plus de code se retrouve dans le fichier `.hpp`.

Le souci, c'est que le fichier `.hpp` est chargé et est recompilée chaque fois qu'il est chargé. Ce qui fait que par exemple, si j'utilise systèmatique une vecteur qui est défini dans le fichier `vector`, je vais recompiler x fois, le nombre de fois que je charge, ce fichier. Cela fait que charger l'ensemble de la STL peut devenir une opération couteuse et allonger inutilement le temps de compilation. 

Ensuite, vous avez vu que nous devons faire attention à ne pas charger dans une unité de compilation deux fois le même fichier header, sinon, nous allons avoir des erreurs indiquant qu'un module est défini deux fois. 

Nous pouvons bien séparer ce qui est visible et ce qui n'est pas visible de l'applicatif. C'est assez important parce que par exemple, nous souhaitons ne pas rendre visible certaines classes utilitaires qui sont des classes utilisées par des classes plus générales qui elles sont visibles. Cependant, si nous devons pour une raison ou une autre fournir le code dans le fichier `.hpp`, ces classes seront visibles, bien que dans l'absolu nous souhaitons le cacher.

C'est pour cela que le module de C/C++ reposant sur les unités de compilation était devenu obsolète et qu'un modèle plus moderne, proche de ce que nous trouvons dans les autres langages était souhaité. Ce modèle devait être statique à la différence des modules dans les langages dynamiques. Il devait cependant être suffissament expressif et surtout permettre une évolution en douceur du modèle d'unité de compilation vers le modèle des modules.

Les avantages des modules sont :

* **Accélération de la compilation**: le chargement d'un module ne prend que le temps de charger le module, plus le temps de procéder à l'analyse syntaxique du code contenu dans le `.hpp`.

* **Supprimer les règles d'écriture ad hoc pour résoudre un problème**: il existe plusieurs méthodes reposant sur les macros pour pouvoir indiquer si un fichier d'entête est lu plusieurs fois d'ignore une partie, si le fichier header est lu par le fichier code associé de modifier certaines déclarations par exemple des variables externes, d'essayer de force un ordre d'initialisation, etc.

* **Supprimer la dichotomie entre fichiers d'entête et de fichier de code**: ceci est particulièrement pertinent parce que le code se trouvait déjà majoritairement dans le fichier d'entête et que la séparation ne faisait plus grand sens.

* **Simplifier la maintenance du code**: il n'est plus nécessaire de redonder l'information entre deux fichiers pour rendre accessible de l'extérieur le code. Par exemple, si je déclarais une fonction dans le fichier `.cpp`, je devais redéclarer cette fonction dans le fichier `.hpp`.

## Comment compile-t-on les modules en C++

Malheureusement le support des modules par les IDE n'est pas encore parfait. Certainement, le support le plus transparent est fourni par [Visual Studio](https://learn.microsoft.com/en-us/cpp/cpp/modules-cpp?view=msvc-170). 

### Compiler sous Visual Studio 

#### Création d'un projet C++ sous Visual Studio

Vous créer un projet C++ et vous indiquez que vous souhaitez utiliser pour ce projet C++20, voir la dernière version en data (qui incluera partie de C++23).

Sous Visual Studio (ainsi que pour le compilateur), le fichier d'exportation d'un module doit avoir l'extension `.ixx`. Ainsi nous allons avoir comme nom de fichier `math.ixx` pour le fichier suivant :


```cpp
export module math

export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}
```

Vous créez donc ce fichier dans la liste des fichiers source.

Vous pouvez désormais modifier le fichier principal, pour includer le code suivant :

```cpp
import math;

int main()
{
    fact(20);
    return 0;
}
```

et c'est tout, c'est terminé !

## Compiler en ligne de commande

### Compiler avec Visual C++

Pour `cl.exe`, l'extension du fichier d'exportation d'un module doit avoir l'extension `.ixx`. Cependant, nous pouvons utiliser aussi un autre nom, mais dans ce cas, il faudra bien spécifier l'extension et le fait que nous précompilons le fichier avec la commande `/module:interface`.

Ainsi nous allons avoir comme nom de fichier `math.ixx` ou `math.cppm` pour le fichier suivant :

```cpp
export module math

export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}
```

Pour compiler, nous allons utiliser les commandes suivantes:

```cmd
cl.exe /std:c++latest /TP /EHsc /MD /c math.cppm /module:interface /Fo: math.obj /module:output math.pcm    // 1
cl.exe /std:c++latest /TP /EHsc /MD /c main.cpp /module:reference math.pcm /Fo: main.obj                 // 2
cl.exe math.obj main.obj
```

La première ligne de commande compile la définition d'interface du module `math.cppm`, la seconde ligne compile le ficher `main.cpp` qui fait référence au fichier `math.pcm` qui contient le résultat de la précompilation du fichier `math.cppm`. La troisième ligne effectue l'édition des liens avec l'ensemble des unités de compilation `math.obj` et `main.obj`. 

### Compiler avec Clang

Comme visual studio, CLang supporte aussi les modules C++. `clang++` suggère d'utiliser `.cppm` comme extension pour les délcarations de modules. Ainsi le fichier précédent au contenu :
```cpp
export module math

export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}
```
aura pour nom : `math.cppm`.

Pour compiler, nous allons utiliser les commandes suivantes:

```bash
clang++ -std=c++20 -fmodules-ts --precompile math.cppm -o math.pcm                   // 1
clang++ -std=c++20 -fmodules-ts -c math.pcm -o math.o                                // 2
clang++ -std=c++20 -fmodules-ts -fprebuilt-module-path=. math.o main.cpp -o main
```

La première ligne de commande compile la définition d'interface du module `math.cppm`, la seconde ligne génère le fichier `math.o` qui contient le code qui peut-être généré qe manière indépendante en compilant le code précompilé de `math.pcm`. Et enfin, on compile `main.cpp` en faisant référence au module précompilé `math.pcm` et `math.o`.

### Compiler avec g++

`g++` ne conseille aucune extension particulière pour les déclarations de module. Dans ce cas, nous pouvons utiliser `math.cpp` pour le fichier contenant :

```cpp
export module math

export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}
```

```bash
g++ -std=c++20 -fmodules-ts -c math.cpp                         // 1
g++ -std=c++20 -fmodules-ts -c main.cc                          // 2
g++ -std=c++20 main.o advanced_mathematics.o -o main
```


# Question n°1

Effectuer la compilation avec votre compilateur et/ou environnement préféré et vérifier que celle-ci se passe correctement.


# Question n°2

Nous souhaitons modifier le code de factoriel. En effet, au lieu d'avoir une définition avec récursion terminale, nous souhaitons que la fonction appelle une fonction interne qui effectue le calcul de manière itérative :


```cpp

inline double fact(int n)
{
    return n == 0 ? 1 : interative_fact(n);

}
inline double interative_fact(int n)
{
    double value = 1.0;
    for(int i = 2; i < n; i ++)
        value *= i;
    return value;
}
```

Modifier le code du fichier `math.cppm` ou `match.ixx` pour que seule la fonction `fact` soit visible.

Compiler et vérifier que le comportement est celui-attendu. Essayer d'accéder à la fonction `interative_fact` et regarder ce qui se passe.

# Question n°3

Nous souhaitons modifier le code de factoriel en paramétrisant celui-ci par le type entier en entrée. Ceci nous donne le code suivant : 

```cpp

inline double fact(std::integral auto n)
{
    return n == 0 ? 1 : interative_fact((int)n);
}

inline long double interative_fact(int n)
{
    double value = 1.0;
    for(int i = 2; i < n; i ++)
        value *= i;
    return value;
}
```

Vérifier que le code fonctionne et qu'il peut-être appelé pour les types `short int`, `short unsigned`, `int`, `long int`.



