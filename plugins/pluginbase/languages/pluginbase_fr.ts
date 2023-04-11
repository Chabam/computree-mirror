<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>PB_StepApplyPointFilters</name>
    <message>
        <source>Groupe</source>
        <translation></translation>
    </message>
    <message>
        <source>Filtres de points</source>
        <translation></translation>
    </message>
    <message>
        <source>Points</source>
        <translation></translation>
    </message>
    <message>
        <source>Item contenant des points</source>
        <translation></translation>
    </message>
    <message>
        <source>Attributs LAS</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepBeginLoopThroughGroups02</name>
    <message>
        <source>Groupe</source>
        <translation>Groupe</translation>
    </message>
    <message>
        <source>Item</source>
        <translation>Item</translation>
    </message>
    <message>
        <source>Boucle standard</source>
        <translation></translation>
    </message>
    <message>
        <source>Nom</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;effectuer des traitements par lots.&lt;br&gt;Elle créée une boucle permettant de traiter successivement les différents éléments sélectionnés en entrée.&lt;br&gt;IMPORTANT : la boucle devra être fermée avec l&apos;étape &quot;Fin de boucle&quot;.&lt;br&gt;&lt;br&gt;Le cas d&apos;utilisation le plus classique est après l&apos;étape &quot;Créer une liste de fichiers&quot;, afin d&apos;itérer sur les fichiers de la liste.</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;L&apos;item choisi définit sur quelle liste d&apos;éléments il faut itérer.&lt;br&gt;Pour cet item il faut choisir un attribut &quot;Nom&quot;, qui permettra de nommer les tours de boucles.&lt;br&gt;Cela permet par exemple de créer dans la boucle des exports adaptatifs, dont les noms de fichiers de sortie seront constitués à partir du nom des items en entrée (nom du fichier en cours en général).</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape génère deux résultats :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Une copie du résultat d&apos;entrée, mais dans laquelle seul l&apos;item de l&apos;itération en cours est disponible,&lt;/li&gt;&lt;li&gt;Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>L&apos;utilisation la plus habituelle de cette étape est de traiter successivement une liste de fichiers. Dans ce cas, la structure du script est la suivante :&lt;ol&gt;&lt;li&gt;Créer une liste de fichier (séléction de la liste des fichiers à parcourir)&lt;/li&gt;&lt;li&gt;&lt;strong&gt;Boucle standard&lt;/strong&gt; (début de la boucle)&lt;/li&gt;&lt;li&gt;Charger les fichiers d&apos;une liste (chargement du fichier correspondant au tour de boucle courant)&lt;/li&gt;&lt;li&gt;... (étapes de traitement et d&apos;export)&lt;/li&gt;&lt;li&gt;Fin de boucle&lt;/li&gt;&lt;/ol&gt;&lt;br&gt;Cependant cette étape est générique et peut donc être utilisée dans de nombreux autres cas.&lt;br&gt;Il est même possible d&apos;emboiter plusieurs boucles, en prenant garde de bien gérer les correspondances de résultats compteurs entre chaque début et fin de boucle.</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepComputePointMetrics</name>
    <message>
        <source>Groupe</source>
        <translation>Groupe</translation>
    </message>
    <message>
        <source>Scène</source>
        <translation>Scène</translation>
    </message>
    <message>
        <source>Métriques de points (XYZ)</source>
        <translation></translation>
    </message>
    <message>
        <source>Points</source>
        <translation></translation>
    </message>
    <message>
        <source>Emprise de la placette</source>
        <translation></translation>
    </message>
    <message>
        <source>Attributs LAS</source>
        <translation></translation>
    </message>
    <message>
        <source>Métriques</source>
        <translation></translation>
    </message>
    <message>
        <source> (%1)</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape regroupe toutes les métriques de points disponibles dans les différents plugins actifs.&lt;br&gt;&lt;br&gt;Dans Computree une &quot;métrique&quot; est un indicateur calculé sur un type de données précis. Les métriques de points sont calculées à partir d&apos;un nuage de points. A minima les coordonnées (x,y,z) des points, et dans certains cas d&apos;autres attributs issus du format standard LAS. De plus, une emprise peut optionnellement être fournie pour sélectionner les points à prendre en compte.&lt;br&gt;&lt;br&gt;&lt;strong&gt;&lt;a href=&quot;#metricsList&quot;&gt;La liste des métriques de points disponibles&lt;/a&gt; est fournie en dernière partie de cette page.&lt;/strong&gt;&lt;br&gt;&lt;br&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Toutes les métriques de points prennent les mêmes données en entrée :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un nuage de points. C&apos;est avec les coordonnées de ce nuage de points que les métriques sont calculées.&lt;/li&gt;&lt;li&gt;Optionnellement une emprise. Si elle est sélectionnée, seuls les points inclus dans cette emprise sont pris en compte pour le calcul.&lt;/li&gt;&lt;li&gt;Optionnellement des attributs LAS. Certaines métriques nécessitent des attributs complémentaires en plus des coordonnées. Si les attributs LAS ne sont pas sélectionnés, ces metriques seront fixées à leur valeur par défaut.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape ajoute au résultat d&apos;entrée un conteneur &quot;métriques&quot;, contenant toutes les métriques calculées. </source>
        <translation></translation>
    </message>
    <message>
        <source>Il faut prendre garde à deux aspects lors de l&apos;utilisation des métriques de points.&lt;br&gt;&lt;br&gt;Premièrement, est-ce que la métrique nécessite les attributs LAS pour son calcul ? Si oui, il faut impérativement sélectionner ces attributs dans les données d&apos;entrée, sous peine d&apos;obtenir la valeur par défaut pour la métrique systématiquement.&lt;br&gt;Les métriques utilisant les attributs LAS, ont le mot clé LAS dans leur intitulé.&lt;br&gt;&lt;br&gt;Deuxièmement, les nuages de points sont originellement codés en altitude absolue. Mais il est fréquent de modifier ces nuages en soutrayant l&apos;altitude du sol, afin d&apos;obtenir des nuages de points en hauteur, où le relief est &quot;retiré&quot;.&lt;br&gt;Il est donc important de savoir si un nuage des points est en Altitude (Alti) ou en Hauteur (Ht).&lt;br&gt;&lt;ul&gt;&lt;li&gt;Certaines métriques sont indifférentes, et fonctionnent dans les deux cas.&lt;/li&gt;&lt;li&gt;D&apos;autres n&apos;ont de sens que pour un nuage en altitude&lt;/li&gt;&lt;li&gt;D&apos;autres n&apos;ont de sens que pour un nuage en hauteur&lt;/li&gt;&lt;/ul&gt;C&apos;est à l&apos;utilisateur de vérifier les métriques adaptées au nuage de points fourni. &lt;br&gt;Pour éclairer ce choix, les titres de métriques contiennent généralement les mots clé Alti, Ht ou les deux. S&apos;il n&apos;y a pas de précision, la métrique fonctionne a priori pour les deux cas (en cas de doute se reporter à sa description ci-dessus).&lt;br&gt;&lt;/div&gt;&lt;div&gt;&lt;h2 id=&quot;metricsList&quot;&gt;Liste des métriques de points disponibles :&lt;/h2&gt;%1</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepComputeRasterMetrics</name>
    <message>
        <source>Groupe</source>
        <translation>Groupe</translation>
    </message>
    <message>
        <source>Métriques de rasters</source>
        <translation></translation>
    </message>
    <message>
        <source>Rasters</source>
        <translation></translation>
    </message>
    <message>
        <source>Raster</source>
        <translation></translation>
    </message>
    <message>
        <source>Emprise de la placette</source>
        <translation></translation>
    </message>
    <message>
        <source>Métriques</source>
        <translation></translation>
    </message>
    <message>
        <source> (%1)</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape regroupe toutes les métriques de raster disponibles dans les différents plugins actifs.&lt;br&gt;&lt;br&gt;Dans Computree une &quot;métrique&quot; est un indicateur calculé sur un type de données précis. Les métriques de raster sont calculées à partir raster (image 2D, où chaque pixel contient une valeur). Une emprise peut optionnellement être fournie pour sélectionner la partie du raster à prendre en compte.&lt;br&gt;&lt;br&gt;&lt;strong&gt;&lt;a href=&quot;#metricsList&quot;&gt;La liste des métriques de raster disponibles&lt;/a&gt; est fournie en dernière partie de cette page.&lt;/strong&gt;&lt;br&gt;&lt;br&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Toutes les métriques de raster prennent les mêmes données en entrée :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un raster à partir duquel les métriques sont calculées.&lt;/li&gt;&lt;li&gt;Optionnellement une emprise. Si elle est sélectionnée, seule la partie du raster incluse dans cette emprise est prise en compte pour le calcul.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape ajoute au résultat d&apos;entrée un conteneur &quot;métriques&quot;, contenant toutes les métriques calculées. </source>
        <translation></translation>
    </message>
    <message>
        <source>Attention : le comportement des métriques peut être influencé par la résolution des rasters fournis.&lt;br&gt;&lt;/div&gt;&lt;div&gt;&lt;h2 id=&quot;metricsList&quot;&gt;Liste des métriques de raster disponibles :&lt;/h2&gt;%1</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepCreatePointFileIndex</name>
    <message>
        <source>Créer des fichiers d&apos;index de points</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>Emprises</source>
        <translation></translation>
    </message>
    <message>
        <source>Groupe Emprise</source>
        <translation></translation>
    </message>
    <message>
        <source>Emprise Réduite</source>
        <translation></translation>
    </message>
    <message>
        <source>Emprise Complète</source>
        <translation></translation>
    </message>
    <message>
        <source>Identifiant</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Groupe Fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier de points</source>
        <translation></translation>
    </message>
    <message>
        <source>Répertoire d&apos;export des fichiers d&apos;index (vide)</source>
        <translation></translation>
    </message>
    <message>
        <source>Vérification des fichiers.</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur de fichier : pas de reader disponible (le fichier a peut-être déjà été chargé).</source>
        <translation></translation>
    </message>
    <message>
        <source>Tous les fichiers d&apos;entrée n&apos;ont pas le même format. Arrêt de l&apos;indexation.</source>
        <translation></translation>
    </message>
    <message>
        <source>Tous les fichiers d&apos;entrée ne sont pas dans le même dossier. Arrêt de l&apos;indexation.</source>
        <translation></translation>
    </message>
    <message>
        <source>Repertoire d&apos;export non choisi. Arrêt de l&apos;indexation.</source>
        <translation></translation>
    </message>
    <message>
        <source>Repertoire d&apos;export inexistant. Arrêt de l&apos;indexation.</source>
        <translation></translation>
    </message>
    <message>
        <source>Initialisation des fichiers d&apos;index.</source>
        <translation></translation>
    </message>
    <message>
        <source>Création des fichiers d&apos;index (%1 fichiers à analyser)</source>
        <translation></translation>
    </message>
    <message>
        <source>Analyse du fichier %1 (%2/%3)</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepCreateReaderList</name>
    <message>
        <source>Créer une liste de fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Choisir les fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun fichier sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier &quot;%1&quot; non accepté par le reader</source>
        <translation></translation>
    </message>
    <message>
        <source>Liste de readers</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Entête</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier %1 inexistant ou non valide</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape créer une liste fichiers. Cette étape ne charge pas les fichiers, elle créée juste les entêtes. Il faut ensuite ajouter l&apos;étape &quot;Charger les fichiers d&apos;une liste&quot; pour les charger, soit directement, soit après un début de boucle. </source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Cette étape génère un résultat contenant la liste des fichiers.&lt;br&gt;Pour chaque fichier deux éléments sont créés :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Une entête contenant les informations générales sur le fichier, dépendant du format. A minima cette entête contient le nom et le chemin du fichier.&lt;/li&gt;&lt;li&gt;Un &quot;reader&quot;, qui est l&apos;objet utilisé pour faire le chargement effectif du fichier dans la suite du script.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>L&apos;utilisation la plus habituelle de cette étape est de traiter successivement une liste de fichiers, à l&apos;aide d&apos;une boucle ajoutée après cette étape. Dans ce cas, la structure du script est la suivante :&lt;ol&gt;&lt;li&gt;&lt;strong&gt;Créer une liste de fichier&lt;/strong&gt; (séléction de la liste des fichiers à parcourir)&lt;/li&gt;&lt;li&gt;Boucle standard (début de la boucle)&lt;/li&gt;&lt;li&gt;Charger les fichiers d&apos;une liste (chargement du fichier correspondant au tour de boucle courant)&lt;/li&gt;&lt;li&gt;... (étapes de traitement et d&apos;export)&lt;/li&gt;&lt;li&gt;Fin de boucle&lt;/li&gt;&lt;/ol&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>Méthode de choix des fichiers :</source>
        <translation></translation>
    </message>
    <message>
        <source>Sélection manuelle dans un répertoire</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette option est sélectionnée, l&apos;utilisateur choisit manuellement les fichiers dans un répertoire.</source>
        <translation></translation>
    </message>
    <message>
        <source>Utilisation d&apos;un fichier contenant les chemins</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette option est sélectionnée, l&apos;utilisateur doit choisir un fichier texte, qui contient sur chaque ligne un chemin de fichier. Cela permet de préparer des lots de fichiers prédéfinis.</source>
        <translation></translation>
    </message>
    <message>
        <source>Choisir le fichier contenant les chemins</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier texte, *.*</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepExportAttributesAsASCII</name>
    <message>
        <source>Export d&apos;attributs - ASCII</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;exporter des attributs d&apos;items situés dans un même groupe. Cette étape fonctionne également au sein d&apos;une boucle, en prenant en compte les tours surccessifs. Tout attribut de n&apos;importe quel item peut être exporté.&lt;br&gt;L&apos;export est réalisé sous forme de fichier texte, avec une ligne par item (et par tour de boucle le cas échéant), et une colonne par attribut. Un seul fichier est produit, regroupant les données de tous les tours si on est dans une boucle.</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Il faut sélectionner les attributs à exporter.&lt;br&gt;Ces attributs peuvent appartenir à plusieurs items, tant que ces items sont dans un même groupe.&lt;br&gt;Lorsque l&apos;export est au sein d&apos;une boucle, il faut également sélectionner le compteur de boucle.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape ne génère pas de nouvelles données.</source>
        <translation></translation>
    </message>
    <message>
        <source>N.B. : les noms d&apos;attributs sont simplifiés : suppression des accents, remplacement des espaces et caractères spéciaux par &quot;_&quot;.</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat</source>
        <translation>Résultat</translation>
    </message>
    <message>
        <source>Item avec des attributs</source>
        <translation></translation>
    </message>
    <message>
        <source>Attribut à exporter</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Export ASCII tabulaire (1 fichier en tout)</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier texte (*.txt)</source>
        <translation></translation>
    </message>
    <message>
        <source>S&apos;il existe déjà, le fichier sera écrasé. Le fichier contiendra les données pour tous les tours de boucle. </source>
        <translation></translation>
    </message>
    <message>
        <source>Supprimer les caractères spéciaux dans les noms de champs</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée tous les caractères accentués seront remplacés par leur version non accentuée, et tous les caractères spéciaux seront remplacés par &quot;_&quot;, dans les noms de métriques</source>
        <translation></translation>
    </message>
    <message>
        <source>Raccourcir les noms de métriques (à 10 caractères)</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée les noms de métriques sont raccourcis si nécessaire pour ne pas dépasser 10 caractères. Dans ce cas un fichier texte est produit contenant les correspondances entre noms complets et noms raccourcis. </source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible de créer le fichier d&apos;export ASCII. Arrêt des traitements.</source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible d&apos;ouvrir le fichier d&apos;export ASCII. Arrêt des traitements.</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepExportAttributesAsRaster</name>
    <message>
        <source>Export d&apos;attributs - Raster</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;exporter des attributs d&apos;items situés dans un même groupe. Cette étape fonctionne également au sein d&apos;une boucle, en prenant en compte les tours surccessifs. Tout attribut de n&apos;importe quel item peut être exporté.&lt;br&gt;L&apos;export est réalisé sous forme de raster, produisant un fichier raster par attribut (et par tour de boucle le cas échéant). Pour que cet export puisse être utilisé, il faut que les données soient organisées sous forme de grille spatiale.</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Il faut sélectionner les attributs à exporter.&lt;br&gt;Ces attributs peuvent appartenir à plusieurs items, tant que ces items sont dans un même groupe.&lt;br&gt;Il faut également séléctionner des attributs pour les coordonnées (X,Y) auxquelles les attributs seront associés spatialement.&lt;br&gt;Il faut de plus sélectionner un objet &quot;Grille de placettes&quot;, par exemple généré par l&apos;étape &quot;Créer une grille de placettes sur l&apos;emprise&quot;.&lt;br&gt;Enfin, lorsque l&apos;export est au sein d&apos;une boucle, il faut sélectionner le compteur de boucle.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape ne génère pas de nouvelles données.</source>
        <translation></translation>
    </message>
    <message>
        <source>N.B. : les noms d&apos;attributs sont simplifiés : suppression des accents, remplacement des espaces et caractères spéciaux par &quot;_&quot;.</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat</source>
        <translation>Résultat</translation>
    </message>
    <message>
        <source>Grille de placettes</source>
        <translation></translation>
    </message>
    <message>
        <source>Item de position (avec XY)</source>
        <translation></translation>
    </message>
    <message>
        <source>X</source>
        <translation></translation>
    </message>
    <message>
        <source>Y</source>
        <translation></translation>
    </message>
    <message>
        <source>Item avec des attributs</source>
        <translation></translation>
    </message>
    <message>
        <source>Attribut à exporter</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Export raster (1 fichier / tour / métrique)</source>
        <translation></translation>
    </message>
    <message>
        <source>Prefixe pour les fichiers exportés</source>
        <translation></translation>
    </message>
    <message>
        <source>Un prefixe optionnel peut être ajouté à tous les noms de fichier, pour par exemple identifier différents lancements du script, exportant dans un même répertoire.</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du format d&apos;export</source>
        <translation></translation>
    </message>
    <message>
        <source>Format raster à utiliser pour les exports.</source>
        <translation></translation>
    </message>
    <message>
        <source>Répertoire d&apos;export (vide de préférence)</source>
        <translation></translation>
    </message>
    <message>
        <source>Le contenu du dossier sélectionné ne sera pas effacé. Cependant pour plus de clarté il est préférable de choisir un dossier vide.</source>
        <translation></translation>
    </message>
    <message>
        <source>Créer un sous-dossier par métrique</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée, un dossier séparé sera créé pour achaque attribut à exporter, contenant chacun un raster par tour de boucle.</source>
        <translation></translation>
    </message>
    <message>
        <source>Supprimer les caractères spéciaux dans les noms de champs</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée tous les caractères accentués seront remplacés par leur version non accentuée, et tous les caractères spéciaux seront remplacés par &quot;_&quot;, dans les noms de métriques</source>
        <translation></translation>
    </message>
    <message>
        <source>Raccourcir les noms de métriques (à 10 caractères)</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée les noms de métriques sont raccourcis si nécessaire pour ne pas dépasser 10 caractères. Dans ce cas un fichier texte est produit contenant les correspondances entre noms complets et noms raccourcis. </source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepExportAttributesAsVector</name>
    <message>
        <source>Export d&apos;attributs - Vectoriel</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;exporter des attributs d&apos;items situés dans un même groupe. Cette étape fonctionne également au sein d&apos;une boucle, en prenant en compte les tours surccessifs. Tout attribut de n&apos;importe quel item peut être exporté.&lt;br&gt;L&apos;export est réalisé sous forme de vecteur (points en 2D avec attributs), produisant un fichier vecteur (par tour de boucle le cas échéant), avec une ligne par item, et une colonne par attribut. Cela nécessite que parmi les attributs figurent des coordonnées (x;y), auquelles assosicer les autres attributs.</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Il faut sélectionner les attributs à exporter.&lt;br&gt;Ces attributs peuvent appartenir à plusieurs items, tant que ces items sont dans un même groupe.&lt;br&gt;Il faut également séléctionner des attributs pour les coordonnées (X,Y) auxquelles les attributs seront associés spatialement.&lt;br&gt;Enfin, lorsque l&apos;export est au sein d&apos;une boucle, il faut sélectionner le compteur de boucle.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape ne génère pas de nouvelles données.</source>
        <translation></translation>
    </message>
    <message>
        <source>N.B. : les noms d&apos;attributs sont simplifiés : suppression des accents, remplacement des espaces et caractères spéciaux par &quot;_&quot;.</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat</source>
        <translation></translation>
    </message>
    <message>
        <source>Item de position (avec XY)</source>
        <translation></translation>
    </message>
    <message>
        <source>X</source>
        <translation></translation>
    </message>
    <message>
        <source>Y</source>
        <translation></translation>
    </message>
    <message>
        <source>Item avec des attributs</source>
        <translation></translation>
    </message>
    <message>
        <source>Attribut à exporter</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Export vectoriel (1 fichier / tour)</source>
        <translation></translation>
    </message>
    <message>
        <source>Prefixe pour les fichiers exportés</source>
        <translation></translation>
    </message>
    <message>
        <source>Un prefixe optionnel peut être ajouté à tous les noms de fichier, pour par exemple identifier différents lancements du script, exportant dans un même répertoire.</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du format d&apos;export</source>
        <translation></translation>
    </message>
    <message>
        <source>Format vecteur à utiliser pour les exports.</source>
        <translation></translation>
    </message>
    <message>
        <source>Répertoire d&apos;export (vide de préférence)</source>
        <translation></translation>
    </message>
    <message>
        <source>Le contenu du dossier sélectionné ne sera pas effacé. Cependant pour plus de clarté il est préférable de choisir un dossier vide.</source>
        <translation></translation>
    </message>
    <message>
        <source>Supprimer les caractères spéciaux dans les noms de champs</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée tous les caractères accentués seront remplacés par leur version non accentuée, et tous les caractères spéciaux seront remplacés par &quot;_&quot;, dans les noms de champs</source>
        <translation></translation>
    </message>
    <message>
        <source>Raccourcir les noms de champs (à 10 caractères)</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée les noms de champs sont raccourcis si nécessaire pour ne pas dépasser 10 caractères. Dans ce cas un fichier texte est produit contenant les correspondances entre noms complets et noms raccourcis. Cette option est fortement recommandée en cas d&apos;export au format ESRI Shapefile. </source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible d&apos;utiliser le format d&apos;export Vectoriel choisi.</source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible de créer la couche &quot;point&quot;.</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepExportAttributesInLoop</name>
    <message>
        <source>Résultat</source>
        <translation>Résultat</translation>
    </message>
    <message>
        <source>Export dans une boucle (cas normal)</source>
        <translation>Export dans une boucle (cas normal, décocher en cas d&apos;usage hors boucle)</translation>
    </message>
    <message>
        <source>Export d&apos;attributs dans une boucle</source>
        <translation></translation>
    </message>
    <message>
        <source>Activer export ASCII tabulaire (1 fichier en tout)</source>
        <translation></translation>
    </message>
    <message>
        <source>Activer</source>
        <translation></translation>
    </message>
    <message>
        <source>Activer export raster (1 fichier / tour / métrique)</source>
        <translation></translation>
    </message>
    <message>
        <source>L&apos;export raster nécessite une grille de placettes (désactiver si pas de résultat valide)</source>
        <translation></translation>
    </message>
    <message>
        <source>Activer export vectoriel (1 fichier / tour)</source>
        <translation></translation>
    </message>
    <message>
        <source>Grille de placettes</source>
        <translation></translation>
    </message>
    <message>
        <source>X</source>
        <translation></translation>
    </message>
    <message>
        <source>Y</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Export ASCII tabulaire (1 fichier en tout)</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier texte (*.txt)</source>
        <translation></translation>
    </message>
    <message>
        <source>Export raster (1 fichier / tour / métrique)</source>
        <translation></translation>
    </message>
    <message>
        <source>Prefixe pour les fichiers exportés</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du format d&apos;export</source>
        <translation></translation>
    </message>
    <message>
        <source>Répertoire d&apos;export (vide de préférence)</source>
        <translation></translation>
    </message>
    <message>
        <source>Créer un sous-dossier par métrique</source>
        <translation></translation>
    </message>
    <message>
        <source>Export vectoriel (1 fichier / tour)</source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible de créer le fichier d&apos;export ASCII. Arrêt des traitements.</source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible d&apos;ouvrir le fichier d&apos;export ASCII. Arrêt des traitements.</source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible d&apos;utiliser le format d&apos;export Vectoriel choisi.</source>
        <translation></translation>
    </message>
    <message>
        <source> : Impossible de créer la couche &quot;point&quot;.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;exporter des données au sein d&apos;une boucle, en prenant en compte les tours surccessifs. Tout attribut de n&apos;importe quel item peut être exporté.&lt;br&gt;Cette étape propose 3 types d&apos;exports différents, pouvant être réalisés en parallèle ou non :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un export sous forme de fichier texte, avec une ligne par item et tour de boucle, et une colonne par attribut. Un seul fichier est produit, regroupant les données de tous les tours de boucle.&lt;/li&gt;&lt;li&gt;Un export sous forme de raster, produisant un fichier raster par attribut et par tour de boucle. Pour que cet export puisse être utilisé, il faut que les données soient organisées sous forme de grille spatiale.&lt;/li&gt;&lt;li&gt;Un export sous forme de vecteur (points en 2D avec attributs), produisant un fichier vecteur par tour de boucle, avec une ligne par item, et une colonne par attribut. Cela nécessite que parmi les attributs figurent des coordonnées (x;y), auquelles assosicer les autres attributs.&lt;/li&gt;&lt;/ul&gt;Il est possible d&apos;utiliser cette étape en dehors d&apos;une boucle, même si ce n&apos;est pas son usage prévu, en sélectionnant l&apos;option correspondante. </source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Le type de données d&apos;entrée nécessaire dépend des exports activés.&lt;br&gt;Dans tous les cas il faut sélectionner les attributs à exporter.&lt;br&gt;Pour les exports raster, il faut sélectionner un objet &quot;Grille de placettes&quot;, par exemple généré par l&apos;étape &quot;Créer une grille de placettes sur l&apos;emprise&quot;.&lt;br&gt;Pour les exports vecteurs (et aussi raster), il faut également séléctionner des attributs pour les coordonnées (x;y) auxquelles les attributs seront associés spatialement.&lt;br&gt;&lt;br&gt;Comme il s&apos;agit d&apos;un export au sein d&apos;une boucle, il faut également sélectionner le compteur de boucle (sauf si l&apos;option d&apos;export hors boucle a été choisi).</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape ne génère pas de nouvelles données.</source>
        <translation></translation>
    </message>
    <message>
        <source>A noter que les trois types d&apos;exports sont indépendants, même s&apos;ils exportent les mêmes attributs.</source>
        <translation></translation>
    </message>
    <message>
        <source>Item de position (avec XY)</source>
        <translation></translation>
    </message>
    <message>
        <source>Item avec des attributs</source>
        <translation></translation>
    </message>
    <message>
        <source>Attribut à exporter</source>
        <translation></translation>
    </message>
    <message>
        <source>S&apos;il existe déjà, le fichier sera écrasé. Le fichier contiendra les données pour tous les tours de boucle. </source>
        <translation></translation>
    </message>
    <message>
        <source>Un prefixe optionnel peut être ajouté à tous les noms de fichier, pour par exemple identifier différents lancements du script, exportant dans un même répertoire.</source>
        <translation></translation>
    </message>
    <message>
        <source>Format raster à utiliser pour les exports.</source>
        <translation></translation>
    </message>
    <message>
        <source>Le contenu du dossier sélectionné ne sera pas effacé. Cependant pour plus de clarté il est préférable de choisir un dossier vide.</source>
        <translation></translation>
    </message>
    <message>
        <source>Si cette case est cochée, un dossier séparé sera créé pour achaque attribut à exporter, contenant chacun un raster par tour de boucle.</source>
        <translation></translation>
    </message>
    <message>
        <source>Format vecteur à utiliser pour les exports.</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepExportPointsByXYArea</name>
    <message>
        <source>Offset sur les coodonnées Xmin et Ymin</source>
        <translation>Offset sur les coodonnées Xmin et Ymin (nommage)</translation>
    </message>
    <message>
        <source>Parmi les points de la scène d&apos;entrée, séléctionne pour chaque emprise la liste des points à exporter.Cet étape fonctionne dans une boucle, gardant le même fichier de sortie pour chaque emprise, pour toutes les scènes d&apos;entrée (une par tour de boucle).</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation></translation>
    </message>
    <message>
        <source>Résultat emprise</source>
        <translation></translation>
    </message>
    <message>
        <source>Emprise</source>
        <translation></translation>
    </message>
    <message>
        <source>Nom</source>
        <translation></translation>
    </message>
    <message>
        <source>Répertoire d&apos;export</source>
        <translation></translation>
    </message>
    <message>
        <source>Suffixe de nom de fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun exporter sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur lors de l&apos;exportation</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur lors de  la finalisation de l&apos;export</source>
        <translation></translation>
    </message>
    <message>
        <source>Export des points de chaque emprise (boucle)</source>
        <translation></translation>
    </message>
    <message>
        <source>Renommer selon les coordonnées : Xmin_Ymin</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepGenericExporter</name>
    <message>
        <source>Exporter sous...</source>
        <translation>Exporter sous...</translation>
    </message>
    <message>
        <source>%1 : %2</source>
        <translation></translation>
    </message>
    <message>
        <source>Exporte un(des) fichier(s) de type :</source>
        <translation></translation>
    </message>
    <message>
        <source>Si coché vous devrez choisir un item de type &quot;compteur&quot; et le nom du fichier contiendra le nom du tour courant.</source>
        <translation></translation>
    </message>
    <message>
        <source>Si coché : <byte value="xd"/>%1</source>
        <translation></translation>
    </message>
    <message>
        <source>Exporter dans...</source>
        <translation></translation>
    </message>
    <message>
        <source>Chaque item sera exporté dans un fichier différent
Le nom du fichier à utiliser sera fonction du choix que vous allez faire ci-dessous.</source>
        <translation></translation>
    </message>
    <message>
        <source>Si coché : 
L&apos;attribut à utiliser appartiendra à l&apos;item à exporter</source>
        <translation></translation>
    </message>
    <message>
        <source>Si coché : 
L&apos;attribut à utiliser appartiendra à un autre item que celui à exporter</source>
        <translation></translation>
    </message>
    <message>
        <source>Un fichier sera créé pour chaque item. Comment déterminer le nom du fichier ?</source>
        <translation></translation>
    </message>
    <message>
        <source>Saisie manuelle d&apos;un nom de base.
Chaque item génére un fichier nommé NomDeBase_xx, avec xx allant de 0 à n. </source>
        <translation></translation>
    </message>
    <message>
        <source>Nom du fichier contenu dans un attribut d&apos;un autre item</source>
        <translation></translation>
    </message>
    <message>
        <source>Exporter un fichier unique</source>
        <translation></translation>
    </message>
    <message>
        <source>Si coché : 
Un unique ficher de sortie sera créé, contenant toutes le données. </source>
        <translation></translation>
    </message>
    <message>
        <source>Exporter un fichier par item</source>
        <translation></translation>
    </message>
    <message>
        <source>Dans le cas de l&apos;export d&apos;un fichier par item, comment déterminer le nom du fichier ?</source>
        <translation></translation>
    </message>
    <message>
        <source>Nom du fichier contenu dans un attribut de l&apos;item</source>
        <translation></translation>
    </message>
    <message>
        <source>Choisir un nom pour le fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Utiliser le nom du tour courant comme nom de fichier (boucles uniquement), ou comme préfixe si export d&apos;un fichier par item</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepGenericLoadFile</name>
    <message>
        <source>Charge un fichier du type</source>
        <translation>Charge un fichier du type</translation>
    </message>
    <message>
        <source>%1 : %2</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepLoadFileByName</name>
    <message>
        <source>Résultat</source>
        <translation>Résultat</translation>
    </message>
    <message>
        <source>Item</source>
        <translation>Item</translation>
    </message>
    <message>
        <source>Créer un sélecteur de fichier par attribut de nom</source>
        <translation></translation>
    </message>
    <message>
        <source>Nom</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Choisir un fichier exemple</source>
        <translation></translation>
    </message>
    <message>
        <source>Le fichier choisi doit :</source>
        <translation></translation>
    </message>
    <message>
        <source>- Etre dans le répertoire des fichiers à charger</source>
        <translation></translation>
    </message>
    <message>
        <source>- Avoir le même format que les fichiers à charger</source>
        <translation></translation>
    </message>
    <message>
        <source>- Avoir la même structure / version que les fichiers à charger</source>
        <translation></translation>
    </message>
    <message>
        <source>Entête</source>
        <translation></translation>
    </message>
    <message>
        <source>Chargement du fichier %1</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier %1 inexistant ou non valide</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape charge l&apos;entête d&apos;un fichier, dont le nom est déterminé par un attribut choisi.&lt;br&gt;L&apos;utilisateur doit choisir un fichier exemple, qui est utilisé pour définir le répertoire de recherche et vérifier le format utilisé.</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;L&apos;attibut sélectionné doit contenir le nom de base du fichier recherché. Si l&apos;attribut contient un chemin complet, cette étape en extraiera uniquement le nom de base (sans chemin d&apos;accès et sans extension).&lt;br&gt;Il s&apos;agit souvent d&apos;un attribut nommé &quot;filename&quot; ou &quot;filepath&quot;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;Cette étape charge uniquement l&apos;entête du fichier. &lt;strong&gt;Il faudra ensuite charger les données du fichier&lt;/strong&gt;, en général à l&apos;aide de l&apos;étape &quot;Charger les fichiers d&apos;une liste&quot;.</source>
        <translation></translation>
    </message>
    <message>
        <source>En général, cette étape est utilisée dans une boucle, afin de charger un fichier correspondant au tour de boucle en cours.&lt;br&gt;Par exemple, si on a une boucle qui charge tour à tour des nuages de points LIDAR, cette étape peut être utilisée pour identifier à chaque tour de boucle le fichier modèle numérique de terrain (MNT) ayant le même nom que le fichier LIDAR en cours, dans un répertoire dédié.</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Ce fichier exemple doit être l&apos;un des fichiers potentiels à charger (peu importe lequel). Il est analysé pour vérifier le format utilisé, mais aussi pour définir dans quel répertoire il faut chercher les fichiers. </source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepLoopOnFileSets</name>
    <message>
        <source>Groupe</source>
        <translation>Groupe</translation>
    </message>
    <message>
        <source>Boucle sur des groupes de fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier texte (*.txt) ; Fichier texte (*.*)</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier ou dossier non sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun lot trouvé dans le fichier &quot;%1&quot;</source>
        <translation></translation>
    </message>
    <message>
        <source>Vérification des fichiers en cours...</source>
        <translation></translation>
    </message>
    <message>
        <source>Annuler</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier &quot;%1&quot; non accepté par le reader</source>
        <translation></translation>
    </message>
    <message>
        <source>Liste de readers</source>
        <translation></translation>
    </message>
    <message>
        <source>Entête</source>
        <translation></translation>
    </message>
    <message>
        <source>Ajout du fichier %1 du lot %2</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier %1 non valide</source>
        <translation></translation>
    </message>
    <message>
        <source>Chemin du fichier contenant en première colonne les noms des lots et en deuxième colonne les noms des fichiers à traiter (le fichier ne doit pas avoir d&apos;en-tête).</source>
        <translation></translation>
    </message>
    <message>
        <source>Chemin du dossier contenant les fichiers à traiter.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape démarre une boucle de script. Pour cela elle utilise un fichier de correspondance entre des noms de groupes et des fichiers.A chaque tour de boucle, cette étape créée une liste des fichiers appartenant à l&apos;un des groupes.&lt;br&gt;Les groupes sont définis dans un fichier de paramétrage ASCII (sans ligne d&apos;entête) où :&lt;ul&gt;&lt;li&gt;La première colone indique le nom des groupes.&lt;/li&gt;&lt;li&gt;La seconde le chemin des fichiers correspondants.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Cette étape génère deux résultats :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.&lt;/li&gt;&lt;li&gt;Un résultat contenant le nom du groupe, ainsi que la liste des fichiers de ce groupe (non chargés à ce stade)&lt;./li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape est utile pour charger des séries de fichiers devant être traités ensembles.&lt;br&gt;Le nom de groupe pourra également être utilisé, par exemple pour charger un autre fichier lié au groupe, en utilisant l&apos;étape &quot;Créer un sélecteur de fichier par attribut de nom&quot;.&lt;br&gt;&lt;br&gt;Un exemple d&apos;utilisation : charger un nuage de point d&apos;une placette LIDAR, ainsi que les nuages de points des arbres de cette placette (préalablement segmentés).&lt;br&gt;Dans le fichier de correspondance on mettra&lt;br&gt;&lt;ul&gt;&lt;li&gt;Le nom dess placette en tant que noms de groupes en première colonne.&lt;/li&gt;&lt;li&gt;Le nom de chaque fichier arbre en tant que noms de fichiers en seconde colonne.&lt;/li&gt;&lt;/ul&gt;Cette étape créera la liste des fichiers pour chacune des placettes à chacun des tours de boucle. Ils pourront ensuite être chargés dans la boucle à l&apos;aide de l&apos;étape &quot;Charger les fichiers d&apos;une liste&quot;.&lt;br&gt;Ensuite, en utilisant l&apos;étape &quot;Créer un sélecteur de fichier par attribut de nom&quot;, suivie de l&apos;étape  &quot;Charger les fichiers d&apos;une liste&quot;, on pourra charger le fichier de la placette, pour peu que celui-ci soit nommé conformément au nom de placette utilisé comme nom de groupe. </source>
        <translation></translation>
    </message>
    <message>
        <source>Le type choisi doit correspondre aux fichiers listés dans le fichier de correspondance. </source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du fichier de correspondance entre groupes et fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>La première colonne doit contenir le nom des groupes, la seconde colonne doit contenir le nom des fichiers du groupe (avec ou sans extension). Le fichier ne doit pas avoir de ligne d&apos;en-tête.</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du dossier contenant les fichiers listés dans le fichier de correspondance</source>
        <translation></translation>
    </message>
    <message>
        <source>Groupes</source>
        <translation></translation>
    </message>
    <message>
        <source>Nom du Groupe</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichiers du groupe</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepLoopOnFiles</name>
    <message>
        <source>Boucle sur les fichiers d&apos;un dossier</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation></translation>
    </message>
    <message>
        <source>Choix du répertoire contenant les fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Dossier vide</source>
        <translation></translation>
    </message>
    <message>
        <source>Aucun fichier compatible dans le dossier</source>
        <translation></translation>
    </message>
    <message>
        <source>Vérification des fichiers en cours...</source>
        <translation></translation>
    </message>
    <message>
        <source>Annuler</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier &quot;%1&quot; non accepté par le reader</source>
        <translation></translation>
    </message>
    <message>
        <source>Liste de readers</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Entête</source>
        <translation></translation>
    </message>
    <message>
        <source>Ajout du fichier %1</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier %1 non valide</source>
        <translation></translation>
    </message>
    <message>
        <source>Chemin du dossier contenant les fichiers à traiter.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;effectuer des traitements par lots.&lt;br&gt;Elle créée une boucle permettant de traiter successivement les fichiers choisis.&lt;br&gt;IMPORTANT : la boucle devra être fermée avec l&apos;étape &quot;Fin de boucle&quot;.&lt;br&gt;&lt;br&gt;Cette étape est en fait une combinaison des étapes &quot;Créer une liste de fichiers&quot; et &quot;Boucle standard&quot;. </source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Cette étape génère deux résultats :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.&lt;/li&gt;&lt;li&gt;Un résultat contenant l&apos;entête du fichier correspondant à ce tour de boucle. &lt;/li&gt;&lt;/ul&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>On préfèrera la combinaison des étapes &quot;Créer une liste de fichiers&quot; et &quot;Boucle standard&quot;, dans les cas où la liste complète des fichiers à charger est également utile dans le script, par exemple s&apos;il faut générer initialement l&apos;ensemble des emprises des fichiers à charger.&lt;br&gt;A l&apos;inverse cette étape est plus simple s&apos;il suffit de parcourir une liste de fichiers séquenciellement. </source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepUseReaderToLoadFiles</name>
    <message>
        <source>Charger les fichiers d&apos;une liste</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichiers</source>
        <translation></translation>
    </message>
    <message>
        <source>Fichier</source>
        <translation></translation>
    </message>
    <message>
        <source>Chargement du fichier %1</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet de charger effectivement un fichier, dont l&apos;entête a préalablement été créée, par exemple avec l&apos;étape &quot;Créer une liste de fichiers&quot;.</source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;L&apos;item choisi définit sur quel fichier il faut charger. Ces fichiers doivent avoir été préalable listés. C&apos;est l&apos;objet &quot;reader&quot; qui est utilisé pour réaliser le chargement.</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape charge les données du fichier. La structure de données dépend du format du fichier.</source>
        <translation></translation>
    </message>
    <message>
        <source>L&apos;utilisation la plus habituelle de charger un fichier par tour de boucle, à partie d&apos;une liste de fichiers préalablement crée. Dans ce cas, la structure du script est la suivante :&lt;ol&gt;&lt;li&gt;Créer une liste de fichier (séléction de la liste des fichiers à parcourir)&lt;/li&gt;&lt;li&gt;Boucle standard (début de la boucle)&lt;/li&gt;&lt;li&gt;&lt;strong&gt;Charger les fichiers d&apos;une liste&lt;/strong&gt; (chargement du fichier correspondant au tour de boucle courant)&lt;/li&gt;&lt;li&gt;... (étapes de traitement et d&apos;export)&lt;/li&gt;&lt;li&gt;Fin de boucle&lt;/li&gt;&lt;/ol&gt;&lt;br&gt;</source>
        <translation></translation>
    </message>
    <message>
        <source>Entête</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PB_StepUserItemSelection</name>
    <message>
        <source>Item</source>
        <translation>Item</translation>
    </message>
    <message>
        <source>Mode manuel</source>
        <translation>Mode manuel</translation>
    </message>
    <message>
        <source>Bienvenue dans le mode manuel de cette étape de filtrage. Veuillez sélectionner les éléments dans la vue graphique puis valider en cliquant sur le pouce en haut de la fenêtre principale. Les éléments sélectionnés seront gardés dans le résultat de sortie.</source>
        <translation></translation>
    </message>
    <message>
        <source>Séléction interactive d&apos;items</source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape permet à l&apos;utilisateur de faire une séléction manuelle interactive des items à conserver. </source>
        <translation></translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Il faut simplement choisir les items parmi lesquels faire la séléction interactive. </source>
        <translation></translation>
    </message>
    <message>
        <source>Cette étape génère une copie du résultat d&apos;entrée, mais où seuls les items sélectionnés sont conservés. Le reste des données n&apos;est pas impacté. </source>
        <translation></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Plugin_Base initialized</source>
        <translation>Plugin_Base initialisé</translation>
    </message>
</context>
</TS>
