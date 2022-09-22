<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US">
<context>
    <name>PB_StepApplyPointFilters</name>
    <message>
        <source>Filtres de points</source>
        <translation>Point filters</translation>
    </message>
    <message>
        <source>Groupe</source>
        <translation>Group</translation>
    </message>
    <message>
        <source>Item contenant des points</source>
        <translation>Item containing points</translation>
    </message>
    <message>
        <source>Attributs LAS</source>
        <translation>LAS attributes</translation>
    </message>
    <message>
        <source>Points</source>
        <translation>Points</translation>
    </message>
</context>
<context>
    <name>PB_StepBeginLoopThroughGroups02</name>
    <message>
        <source>Groupe</source>
        <translation>Group</translation>
    </message>
    <message>
        <source>Item</source>
        <translation>Item</translation>
    </message>
    <message>
        <source>Nom</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Boucle standard</source>
        <translation>Standard loop</translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;effectuer des traitements par lots.&lt;br&gt;Elle créée une boucle permettant de traiter successivement les différents éléments sélectionnés en entrée.&lt;br&gt;IMPORTANT : la boucle devra être fermée avec l&apos;étape &quot;Fin de boucle&quot;.&lt;br&gt;&lt;br&gt;Le cas d&apos;utilisation le plus classique est après l&apos;étape &quot;Créer une liste de fichiers&quot;, afin d&apos;itérer sur les fichiers de la liste.</source>
        <translation>This step allows you to perform batch processing.&lt;br&gt;It creates a loop that allows you to successively process the different items selected as input.&lt;br&gt;IMPORTANT: the loop will have to be closed with the &quot;End of loop&quot; step.&lt;br&gt;&lt;br&gt;The most classic use case is after the &quot;Create a list of files&quot; step, in order to iterate on the files in the list.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;L&apos;item choisi définit sur quelle liste d&apos;éléments il faut itérer.&lt;br&gt;Pour cet item il faut choisir un attribut &quot;Nom&quot;, qui permettra de nommer les tours de boucles.&lt;br&gt;Cela permet par exemple de créer dans la boucle des exports adaptatifs, dont les noms de fichiers de sortie seront constitués à partir du nom des items en entrée (nom du fichier en cours en général).</source>
        <translation>&lt;br&gt;&lt;br&gt;The chosen item defines on which list of elements it is necessary to iterate.&lt;br&gt;For this item it is necessary to choose a &quot;Name&quot; attribute, which will allow to name the loop turns.&lt;br&gt;This allows for example to create adaptive exports in the loop, whose output file names will be constituted from the name of the input items (name of the current file in general).</translation>
    </message>
    <message>
        <source>Cette étape génère deux résultats :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Une copie du résultat d&apos;entrée, mais dans laquelle seul l&apos;item de l&apos;itération en cours est disponible,&lt;/li&gt;&lt;li&gt;Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>This step generates two results:&lt;br&gt;&lt;ul&gt;&lt;li&gt;A copy of the input result, but in which only the item of the current iteration is available,&lt;/li&gt;&lt;li&gt;A Counter result, allowing the management of the loop. This counter is in particular used to know the name of the current turn.&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>L&apos;utilisation la plus habituelle de cette étape est de traiter successivement une liste de fichiers. Dans ce cas, la structure du script est la suivante :&lt;ol&gt;&lt;li&gt;Créer une liste de fichier (séléction de la liste des fichiers à parcourir)&lt;/li&gt;&lt;li&gt;&lt;strong&gt;Boucle standard&lt;/strong&gt; (début de la boucle)&lt;/li&gt;&lt;li&gt;Charger les fichiers d&apos;une liste (chargement du fichier correspondant au tour de boucle courant)&lt;/li&gt;&lt;li&gt;... (étapes de traitement et d&apos;export)&lt;/li&gt;&lt;li&gt;Fin de boucle&lt;/li&gt;&lt;/ol&gt;&lt;br&gt;Cependant cette étape est générique et peut donc être utilisée dans de nombreux autres cas.&lt;br&gt;Il est même possible d&apos;emboiter plusieurs boucles, en prenant garde de bien gérer les correspondances de résultats compteurs entre chaque début et fin de boucle.</source>
        <translation>The most usual use of this step is to successively process a list of files. In this case, the structure of the script is as follows:&lt;ol&gt;&lt;li&gt;Create a file list (select the list of files to be browsed)&lt;/li&gt;&lt;li&gt;&lt;strong&gt;Standard loop&lt;/strong&gt; (start of the loop)&lt;/li&gt;&lt;li&gt;Load files from a list (load the file corresponding to the current loop turn)&lt;/li&gt;&lt;li&gt;... (processing and export steps)&lt;/li&gt;&lt;li&gt;End of loop&lt;/li&gt;&lt;/ol&gt;&lt;br&gt;However, this step is generic and can therefore be used in many other cases.&lt;br&gt;It is even possible to nest several loops, taking care to manage the counter results correspondences between each loop start and end.</translation>
    </message>
</context>
<context>
    <name>PB_StepComputePointMetrics</name>
    <message>
        <source>Métriques de points (XYZ)</source>
        <translation>Points metrics (XYZ)</translation>
    </message>
    <message>
        <source>Groupe</source>
        <translation>Group</translation>
    </message>
    <message>
        <source>Scène</source>
        <translation>Scene</translation>
    </message>
    <message>
        <source>Emprise de la placette</source>
        <translation>Plot extent shape</translation>
    </message>
    <message>
        <source>Attributs LAS</source>
        <translation>LAS attributes</translation>
    </message>
    <message>
        <source>Métriques</source>
        <translation>Metrics</translation>
    </message>
    <message>
        <source>Points</source>
        <translation>Points</translation>
    </message>
    <message>
        <source> (%1)</source>
        <translation> (%1)</translation>
    </message>
    <message>
        <source>Cette étape regroupe toutes les métriques de points disponibles dans les différents plugins actifs.&lt;br&gt;&lt;br&gt;Dans Computree une &quot;métrique&quot; est un indicateur calculé sur un type de données précis. Les métriques de points sont calculées à partir d&apos;un nuage de points. A minima les coordonnées (x,y,z) des points, et dans certains cas d&apos;autres attributs issus du format standard LAS. De plus, une emprise peut optionnellement être fournie pour sélectionner les points à prendre en compte.&lt;br&gt;&lt;br&gt;&lt;strong&gt;&lt;a href=&quot;#metricsList&quot;&gt;La liste des métriques de points disponibles&lt;/a&gt; est fournie en dernière partie de cette page.&lt;/strong&gt;&lt;br&gt;&lt;br&gt;</source>
        <translation>This step provides all the point metrics available in the various active plugins.&lt;br&gt;&lt;br&gt;In Computree a &quot;metric&quot; is an indicator calculated on a specific type of data. Point metrics are calculated from a point cloud. At a minimum the (x,y,z) coordinates of the points, and in some cases other attributes from the standard LAS format. In addition, an extent can optionally be provided to select the points to be considered.&lt;br&gt;&lt;br&gt;&lt;strong&gt;&lt;a href=&quot;#metricsList&quot;&gt;The list of available point metrics&lt;/a&gt; is provided in the last part of this page.&lt;/strong&gt;&lt;br&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Toutes les métriques de points prennent les mêmes données en entrée :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un nuage de points. C&apos;est avec les coordonnées de ce nuage de points que les métriques sont calculées.&lt;/li&gt;&lt;li&gt;Optionnellement une emprise. Si elle est sélectionnée, seuls les points inclus dans cette emprise sont pris en compte pour le calcul.&lt;/li&gt;&lt;li&gt;Optionnellement des attributs LAS. Certaines métriques nécessitent des attributs complémentaires en plus des coordonnées. Si les attributs LAS ne sont pas sélectionnés, ces metriques seront fixées à leur valeur par défaut.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;br&gt;&lt;br&gt;All point metrics take the same input data:&lt;br&gt;&lt;ul&gt;&lt;li&gt;A point cloud. It is with the coordinates of this point cloud that the metrics are calculated.&lt;/li&gt;&lt;li&gt;Optionally an extent. If selected, only the points included in this extent are taken into account for the calculation.&lt;/li&gt;&lt;li&gt;Optionally LAS attributes. Some metrics require additional attributes in addition to the coordinates. If LAS attributes are not selected, these metrics will be set to their default value.&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>Cette étape ajoute au résultat d&apos;entrée un conteneur &quot;métriques&quot;, contenant toutes les métriques calculées. </source>
        <translation>This step adds a &quot;metrics&quot; container to the input result, containing all calculated metrics.</translation>
    </message>
    <message>
        <source>Il faut prendre garde à deux aspects lors de l&apos;utilisation des métriques de points.&lt;br&gt;&lt;br&gt;Premièrement, est-ce que la métrique nécessite les attributs LAS pour son calcul ? Si oui, il faut impérativement sélectionner ces attributs dans les données d&apos;entrée, sous peine d&apos;obtenir la valeur par défaut pour la métrique systématiquement.&lt;br&gt;Les métriques utilisant les attributs LAS, ont le mot clé LAS dans leur intitulé.&lt;br&gt;&lt;br&gt;Deuxièmement, les nuages de points sont originellement codés en altitude absolue. Mais il est fréquent de modifier ces nuages en soutrayant l&apos;altitude du sol, afin d&apos;obtenir des nuages de points en hauteur, où le relief est &quot;retiré&quot;.&lt;br&gt;Il est donc important de savoir si un nuage des points est en Altitude (Alti) ou en Hauteur (Ht).&lt;br&gt;&lt;ul&gt;&lt;li&gt;Certaines métriques sont indifférentes, et fonctionnent dans les deux cas.&lt;/li&gt;&lt;li&gt;D&apos;autres n&apos;ont de sens que pour un nuage en altitude&lt;/li&gt;&lt;li&gt;D&apos;autres n&apos;ont de sens que pour un nuage en hauteur&lt;/li&gt;&lt;/ul&gt;C&apos;est à l&apos;utilisateur de vérifier les métriques adaptées au nuage de points fourni. &lt;br&gt;Pour éclairer ce choix, les titres de métriques contiennent généralement les mots clé Alti, Ht ou les deux. S&apos;il n&apos;y a pas de précision, la métrique fonctionne a priori pour les deux cas (en cas de doute se reporter à sa description ci-dessus).&lt;br&gt;&lt;/div&gt;&lt;div&gt;&lt;h2 id=&quot;metricsList&quot;&gt;Liste des métriques de points disponibles :&lt;/h2&gt;%1</source>
        <translation>There are two aspects to be aware of when using point metrics.&lt;br&gt;&lt;br&gt;Firstly, does the metric require the LAS attributes for its calculation? If so, these attributes must be selected in the input data, otherwise the default value for the metric will be obtained systematically.&lt;br&gt;The metrics using LAS attributes, have the keyword LAS in their title.&lt;br&gt;&lt;br&gt;Secondly, point clouds are originally coded in absolute altitude. But it is common to modify these clouds by substracting the ground elevation, in order to obtain point clouds in height, where the relief is &quot;removed&quot;.&lt;br&gt;It is therefore important to know whether a point cloud is in Altitude (Alti) or in Height (Ht).&lt;br&gt;&lt;ul&gt;&lt;li&gt;Some metrics are indifferent, and work in both cases.&lt;/li&gt;&lt;li&gt;Other metrics only make sense for an Altitude cloud&lt;/li&gt;&lt;li&gt;Other metrics only make sense for a Height cloud&lt;/li&gt;&lt;/ul&gt;It is up to the user to check which metrics are suitable for the point cloud provided. &lt;br&gt;To inform this choice, metric titles usually contain the keywords Alti, Ht or both. If there is no precision, the metric works a priori for both cases (in case of doubt refer to its description above).&lt;br&gt;&lt;/div&gt;&lt;div&gt;&lt;h2 id=&quot;metricsList&quot;&gt;List of available point metrics:&lt;/h2&gt;%1</translation>
    </message>
</context>
<context>
    <name>PB_StepComputeRasterMetrics</name>
    <message>
        <source>Métriques de rasters</source>
        <translation>Rasters metrics</translation>
    </message>
    <message>
        <source>Groupe</source>
        <translation>Group</translation>
    </message>
    <message>
        <source>Emprise de la placette</source>
        <translation>Plot extent shape</translation>
    </message>
    <message>
        <source>Métriques</source>
        <translation>Metric</translation>
    </message>
    <message>
        <source>Rasters</source>
        <translation>Rasters</translation>
    </message>
    <message>
        <source>Raster</source>
        <translation>Raster</translation>
    </message>
    <message>
        <source> (%1)</source>
        <translation> (%1)</translation>
    </message>
    <message>
        <source>Cette étape regroupe toutes les métriques de raster disponibles dans les différents plugins actifs.&lt;br&gt;&lt;br&gt;Dans Computree une &quot;métrique&quot; est un indicateur calculé sur un type de données précis. Les métriques de raster sont calculées à partir raster (image 2D, où chaque pixel contient une valeur). Une emprise peut optionnellement être fournie pour sélectionner la partie du raster à prendre en compte.&lt;br&gt;&lt;br&gt;&lt;strong&gt;&lt;a href=&quot;#metricsList&quot;&gt;La liste des métriques de raster disponibles&lt;/a&gt; est fournie en dernière partie de cette page.&lt;/strong&gt;&lt;br&gt;&lt;br&gt;</source>
        <translation>This step provides all the raster metrics available in the different active plugins.&lt;br&gt;&lt;br&gt;In Computree a &quot;metric&quot; is an indicator calculated on a specific type of data. Raster metrics are calculated from raster (2D image, where each pixel contains a value). An extent can optionally be provided to select the part of the raster to be considered.&lt;br&gt;&lt;br&gt;&lt;strong&gt;&lt;a href=&quot;#metricsList&quot;&gt;The list of available raster metrics&lt;/a&gt; is provided in the last part of this page.&lt;/strong&gt;&lt;br&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Toutes les métriques de raster prennent les mêmes données en entrée :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un raster à partir duquel les métriques sont calculées.&lt;/li&gt;&lt;li&gt;Optionnellement une emprise. Si elle est sélectionnée, seule la partie du raster incluse dans cette emprise est prise en compte pour le calcul.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;br&gt;&lt;br&gt;All raster metrics take the same input data:&lt;br&gt;&lt;ul&gt;&lt;li&gt;A raster from which the metrics are calculated.&lt;/li&gt;&lt;li&gt;Optionally an extent. If selected, only the part of the raster included in this extent is taken into account for the calculation.&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>Cette étape ajoute au résultat d&apos;entrée un conteneur &quot;métriques&quot;, contenant toutes les métriques calculées. </source>
        <translation>This step adds a &quot;metrics&quot; container to the input result, containing all calculated metrics.</translation>
    </message>
    <message>
        <source>Attention : le comportement des métriques peut être influencé par la résolution des rasters fournis.&lt;br&gt;&lt;/div&gt;&lt;div&gt;&lt;h2 id=&quot;metricsList&quot;&gt;Liste des métriques de raster disponibles :&lt;/h2&gt;%1</source>
        <translation>Warning: the behaviour of the metrics may be influenced by the resolution of the rasters provided.&lt;br&gt;&lt;/div&gt;&lt;div&gt;&lt;h2 id=&quot;metricsList&quot;&gt;List of available raster metrics:&lt;/h2&gt;%1</translation>
    </message>
</context>
<context>
    <name>PB_StepCreateReaderList</name>
    <message>
        <source>Choix du type de fichier</source>
        <translation>Choose file type</translation>
    </message>
    <message>
        <source>Choisir les fichiers</source>
        <translation>Choose files</translation>
    </message>
    <message>
        <source>Liste de readers</source>
        <translation>Readers list</translation>
    </message>
    <message>
        <source>Fichier</source>
        <translation>File</translation>
    </message>
    <message>
        <source>Entête</source>
        <translation>Header</translation>
    </message>
    <message>
        <source>Fichier %1 inexistant ou non valide</source>
        <translation>Fichier %1 not existing or not valid</translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Créer une liste de fichiers</source>
        <translation>Create a list of files</translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation>No reader selected</translation>
    </message>
    <message>
        <source>Aucun fichier sélectionné</source>
        <translation>No file selected</translation>
    </message>
    <message>
        <source>Fichier &quot;%1&quot; non accepté par le reader</source>
        <translation>File &quot;%1&quot; not accepted by the reader</translation>
    </message>
    <message>
        <source>Reader</source>
        <translation>Reader</translation>
    </message>
    <message>
        <source>Cette étape créer une liste fichiers. Cette étape ne charge pas les fichiers, elle créée juste les entêtes. Il faut ensuite ajouter l&apos;étape &quot;Charger les fichiers d&apos;une liste&quot; pour les charger, soit directement, soit après un début de boucle. </source>
        <translation>This step creates a file list. This step does not load the files, it just creates the headers. You then need to add the &quot;Load files from a list&quot; step to load them, either directly or after a loop start.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Cette étape génère un résultat contenant la liste des fichiers.&lt;br&gt;Pour chaque fichier deux éléments sont créés :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Une entête contenant les informations générales sur le fichier, dépendant du format. A minima cette entête contient le nom et le chemin du fichier.&lt;/li&gt;&lt;li&gt;Un &quot;reader&quot;, qui est l&apos;objet utilisé pour faire le chargement effectif du fichier dans la suite du script.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;br&gt;&lt;br&gt;This step generates a result containing the list of files.&lt;br&gt;For each file two elements are created:&lt;br&gt;&lt;ul&gt;&lt;li&gt;A header containing general information about the file, depending on the format. At least this header contains the name and path of the file.&lt;/li&gt;&lt;li&gt;A &quot;reader&quot;, which is the object used to actually load the file in the rest of the script.&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>L&apos;utilisation la plus habituelle de cette étape est de traiter successivement une liste de fichiers, à l&apos;aide d&apos;une boucle ajoutée après cette étape. Dans ce cas, la structure du script est la suivante :&lt;ol&gt;&lt;li&gt;&lt;strong&gt;Créer une liste de fichier&lt;/strong&gt; (séléction de la liste des fichiers à parcourir)&lt;/li&gt;&lt;li&gt;Boucle standard (début de la boucle)&lt;/li&gt;&lt;li&gt;Charger les fichiers d&apos;une liste (chargement du fichier correspondant au tour de boucle courant)&lt;/li&gt;&lt;li&gt;... (étapes de traitement et d&apos;export)&lt;/li&gt;&lt;li&gt;Fin de boucle&lt;/li&gt;&lt;/ol&gt;</source>
        <translation>The most common use of this step is to successively process a list of files, using a loop added after this step. In this case, the structure of the script is as follows:&lt;ol&gt;&lt;li&gt;&lt;strong&gt;Create a file list&lt;/strong&gt; (select the list of files to be browsed)&lt;/li&gt;&lt;li&gt;Standard loop (start of the loop)&lt;/li&gt;&lt;li&gt;Load files from a list (load the file corresponding to the current loop turn)&lt;/li&gt;&lt;li&gt;... (processing and export steps)&lt;/li&gt;&lt;li&gt;End of loop&lt;/li&gt;&lt;/ol&gt;</translation>
    </message>
    <message>
        <source>Méthode de choix des fichiers :</source>
        <translation>Method for choosing files :</translation>
    </message>
    <message>
        <source>Sélection manuelle dans un répertoire</source>
        <translation>Manual selection in a directory</translation>
    </message>
    <message>
        <source>Si cette option est sélectionnée, l&apos;utilisateur choisit manuellement les fichiers dans un répertoire.</source>
        <translation>If this option is selected, the user manually selects the files in a directory.</translation>
    </message>
    <message>
        <source>Utilisation d&apos;un fichier contenant les chemins</source>
        <translation>Use of a file containing the paths</translation>
    </message>
    <message>
        <source>Si cette option est sélectionnée, l&apos;utilisateur doit choisir un fichier texte, qui contient sur chaque ligne un chemin de fichier. Cela permet de préparer des lots de fichiers prédéfinis.</source>
        <translation>If this option is selected, the user must choose a text file, which contains on each line a file path. This allows the preparation of predefined batches of files.</translation>
    </message>
    <message>
        <source>Choisir le fichier contenant les chemins</source>
        <translation>Choose the file containing the paths</translation>
    </message>
    <message>
        <source>Fichier texte, *.*</source>
        <translation>Text file, *.*</translation>
    </message>
</context>
<context>
    <name>PB_StepExportAttributesInLoop</name>
    <message>
        <source>Export d&apos;attributs dans une boucle</source>
        <translation>Attributes export in loop</translation>
    </message>
    <message>
        <source>Résultat</source>
        <translation>Result</translation>
    </message>
    <message>
        <source>Grille de placettes</source>
        <translation>Plots grid</translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation>Counter result</translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation>Counter</translation>
    </message>
    <message>
        <source>Activer export ASCII tabulaire (1 fichier en tout)</source>
        <translation>Activate ASCII tabular export (only one file for all loops turns)</translation>
    </message>
    <message>
        <source>Activer</source>
        <translation>Activate</translation>
    </message>
    <message>
        <source>Activer export raster (1 fichier / tour / métrique)</source>
        <translation>Activate raster export (1 file / loop turn / metric)</translation>
    </message>
    <message>
        <source>L&apos;export raster nécessite une grille de placettes (désactiver si pas de résultat valide)</source>
        <translation>The raster export needs a plots grid to work (don&apos;t activate if not present in input results)</translation>
    </message>
    <message>
        <source>Activer export vectoriel (1 fichier / tour)</source>
        <translation>Activate vector export (1 file / loop turn)</translation>
    </message>
    <message>
        <source>Export dans une boucle (cas normal)</source>
        <translation>Export in loop (standard case, uncheck if used outside of loop)</translation>
    </message>
    <message>
        <source>Choix du fichier</source>
        <translation>File choice</translation>
    </message>
    <message>
        <source>Fichier texte (*.txt)</source>
        <translation>Text file (*.txt)</translation>
    </message>
    <message>
        <source>Export raster (1 fichier / tour / métrique)</source>
        <translation>Raster export (1 file / loop turn / metric)</translation>
    </message>
    <message>
        <source>Prefixe pour les fichiers exportés</source>
        <translation>Prefix for exported files (optionnal)</translation>
    </message>
    <message>
        <source>Choix du format d&apos;export</source>
        <translation>Choose export format</translation>
    </message>
    <message>
        <source>Répertoire d&apos;export (vide de préférence)</source>
        <translation>Export folder (empty is better)</translation>
    </message>
    <message>
        <source>Créer un sous-dossier par métrique</source>
        <translation>Create one folder for each metric</translation>
    </message>
    <message>
        <source>Export vectoriel (1 fichier / tour)</source>
        <translation>Vector export (1 file / loop turn)</translation>
    </message>
    <message>
        <source>X</source>
        <translation>X</translation>
    </message>
    <message>
        <source>Y</source>
        <translation>Y</translation>
    </message>
    <message>
        <source>Export ASCII tabulaire (1 fichier en tout)</source>
        <translation>Tabular ASCII export (1 file in total)</translation>
    </message>
    <message>
        <source> : Impossible de créer le fichier d&apos;export ASCII. Arrêt des traitements.</source>
        <translation> : Unable to create the ASCII export file. Processing stopped.</translation>
    </message>
    <message>
        <source> : Impossible d&apos;ouvrir le fichier d&apos;export ASCII. Arrêt des traitements.</source>
        <translation> : Unable to open the ASCII export file. Processing stopped.</translation>
    </message>
    <message>
        <source> : Impossible d&apos;utiliser le format d&apos;export Vectoriel choisi.</source>
        <translation> : Cannot use the chosen Vector export format.</translation>
    </message>
    <message>
        <source> : Impossible de créer la couche &quot;point&quot;.</source>
        <translation> : Impossible to create the &quot;point&quot; layer.</translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;exporter des données au sein d&apos;une boucle, en prenant en compte les tours surccessifs. Tout attribut de n&apos;importe quel item peut être exporté.&lt;br&gt;Cette étape propose 3 types d&apos;exports différents, pouvant être réalisés en parallèle ou non :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un export sous forme de fichier texte, avec une ligne par item et tour de boucle, et une colonne par attribut. Un seul fichier est produit, regroupant les données de tous les tours de boucle.&lt;/li&gt;&lt;li&gt;Un export sous forme de raster, produisant un fichier raster par attribut et par tour de boucle. Pour que cet export puisse être utilisé, il faut que les données soient organisées sous forme de grille spatiale.&lt;/li&gt;&lt;li&gt;Un export sous forme de vecteur (points en 2D avec attributs), produisant un fichier vecteur par tour de boucle, avec une ligne par item, et une colonne par attribut. Cela nécessite que parmi les attributs figurent des coordonnées (x;y), auquelles assosicer les autres attributs.&lt;/li&gt;&lt;/ul&gt;Il est possible d&apos;utiliser cette étape en dehors d&apos;une boucle, même si ce n&apos;est pas son usage prévu, en sélectionnant l&apos;option correspondante. </source>
        <translation>This step allows you to export data within a loop, taking into account the successive turns. Any attribute of any item can be exported.&lt;br&gt;This step offers 3 different types of exports, which can be performed in parallel or not:&lt;br&gt;&lt;ul&gt;&lt;li&gt;An export in the form of a text file, with one line per item and per loop turn, and one column per attribute. A single file is produced, grouping the data from all the loop turns.&lt;/li&gt;&lt;li&gt;An export in raster form, producing one raster file per attribute and per loop turn. For this export to be used, the data must be organised as a spatial grid.&lt;/li&gt;&lt;li&gt;An export as a vector (2D points with attributes), producing one vector file per loop turn, with one row per item, and one column per attribute. This requires that the attributes include (x;y) coordinates, to which the other attributes are assigned.&lt;/li&gt;&lt;/ul&gt;It is possible to use this step outside of a loop, even if this is not its intended use, by selecting the corresponding option.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Le type de données d&apos;entrée nécessaire dépend des exports activés.&lt;br&gt;Dans tous les cas il faut sélectionner les attributs à exporter.&lt;br&gt;Pour les exports raster, il faut sélectionner un objet &quot;Grille de placettes&quot;, par exemple généré par l&apos;étape &quot;Créer une grille de placettes sur l&apos;emprise&quot;.&lt;br&gt;Pour les exports vecteurs (et aussi raster), il faut également séléctionner des attributs pour les coordonnées (x;y) auxquelles les attributs seront associés spatialement.&lt;br&gt;&lt;br&gt;Comme il s&apos;agit d&apos;un export au sein d&apos;une boucle, il faut également sélectionner le compteur de boucle (sauf si l&apos;option d&apos;export hors boucle a été choisi).</source>
        <translation>&lt;br&gt;&lt;br&gt;The type of input data required depends on the exports activated.&lt;br&gt;In all cases the attributes to be exported must be selected.&lt;br&gt;For raster exports, a &quot;Plot grid&quot; object must be selected, for example generated by the step &quot;Create a plot grid on the extent&quot;.&lt;br&gt;For vector (and also raster) exports, attributes must also be selected for the coordinates (x;y) to which the attributes will be spatially associated.&lt;br&gt;&lt;br&gt;As this is an export within a loop, the loop counter must also be selected (unless the off-loop export option has been chosen).</translation>
    </message>
    <message>
        <source>Cette étape ne génère pas de nouvelles données.</source>
        <translation>This step does not generate new data.</translation>
    </message>
    <message>
        <source>A noter que les trois types d&apos;exports sont indépendants, même s&apos;ils exportent les mêmes attributs.</source>
        <translation>Note that the three types of exports are independent, even if they export the same attributes.</translation>
    </message>
    <message>
        <source>Item de position (avec XY)</source>
        <translation>Position item (with XY)</translation>
    </message>
    <message>
        <source>Item avec des attributs</source>
        <translation>Item with attributes</translation>
    </message>
    <message>
        <source>Attribut à exporter</source>
        <translation>Attribute to be exported</translation>
    </message>
    <message>
        <source>S&apos;il existe déjà, le fichier sera écrasé. Le fichier contiendra les données pour tous les tours de boucle. </source>
        <translation>If it already exists, the file will be overwritten. The file will contain the data for all loop turns.</translation>
    </message>
    <message>
        <source>Un prefixe optionnel peut être ajouté à tous les noms de fichier, pour par exemple identifier différents lancements du script, exportant dans un même répertoire.</source>
        <translation>An optional prefix can be added to all file names, for example to identify different runs of the script, exporting to the same directory.</translation>
    </message>
    <message>
        <source>Format raster à utiliser pour les exports.</source>
        <translation>Raster format to be used for exports.</translation>
    </message>
    <message>
        <source>Le contenu du dossier sélectionné ne sera pas effacé. Cependant pour plus de clarté il est préférable de choisir un dossier vide.</source>
        <translation>The contents of the selected folder will not be deleted. However, for clarity it is better to choose an empty folder.</translation>
    </message>
    <message>
        <source>Si cette case est cochée, un dossier séparé sera créé pour achaque attribut à exporter, contenant chacun un raster par tour de boucle.</source>
        <translation>If this box is checked, a separate folder will be created for each attribute to be exported, each containing one raster per loop turn.</translation>
    </message>
    <message>
        <source>Format vecteur à utiliser pour les exports.</source>
        <translation>Vector format to be used for exports.</translation>
    </message>
</context>
<context>
    <name>PB_StepExportPointsByXYArea</name>
    <message>
        <source>Parmi les points de la scène d&apos;entrée, séléctionne pour chaque emprise la liste des points à exporter.Cet étape fonctionne dans une boucle, gardant le même fichier de sortie pour chaque emprise, pour toutes les scènes d&apos;entrée (une par tour de boucle).</source>
        <translation>Among input scene points, this step select for each input extent, the points ot export contained in this extent. It is working in a loop, keeping same output file for each extent, cumulating points of all input scenes (one for each loop turn).</translation>
    </message>
    <message>
        <source>Résultat compteur</source>
        <translation>Counter result</translation>
    </message>
    <message>
        <source>Compteur</source>
        <translation>Counter</translation>
    </message>
    <message>
        <source>Emprise</source>
        <translation>Entent</translation>
    </message>
    <message>
        <source>Nom</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Répertoire d&apos;export</source>
        <translation>Export directory</translation>
    </message>
    <message>
        <source>Suffixe de nom de fichier</source>
        <translation>Suffix for file name</translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation>Choose file type</translation>
    </message>
    <message>
        <source>Offset sur les coodonnées Xmin et Ymin</source>
        <translation>Offset on coordinates Xmin and Ymin (naming)</translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Résultat emprise</source>
        <translation>Extent result</translation>
    </message>
    <message>
        <source>Aucun exporter sélectionné</source>
        <translation>No export selected</translation>
    </message>
    <message>
        <source>Erreur lors de l&apos;exportation</source>
        <translation>Error during export</translation>
    </message>
    <message>
        <source>Erreur lors de  la finalisation de l&apos;export</source>
        <translation>Error while finalising the export</translation>
    </message>
    <message>
        <source>Export des points de chaque emprise (boucle)</source>
        <translation>Export of the points of each extent (loop)</translation>
    </message>
    <message>
        <source>Renommer selon les coordonnées : Xmin_Ymin</source>
        <translation>Rename according to coordinates: Xmin_Ymin</translation>
    </message>
</context>
<context>
    <name>PB_StepGenericExporter</name>
    <message>
        <source>Exporte un(des) fichier(s) de type :</source>
        <translation>Export one(many) file(s) of type:</translation>
    </message>
    <message>
        <source>Exporter dans...</source>
        <translation>Export in ...</translation>
    </message>
    <message>
        <source>Exporter sous...</source>
        <translation>Export as...</translation>
    </message>
    <message>
        <source>%1 : %2</source>
        <translation>%1 : %2</translation>
    </message>
    <message>
        <source>Si coché vous devrez choisir un item de type &quot;compteur&quot; et le nom du fichier contiendra le nom du tour courant.</source>
        <translation>If checked you will have to choose an item of type &quot;counter&quot; and the file name will contain the name of the current turn.</translation>
    </message>
    <message>
        <source>Si coché : <byte value="xd"/>%1</source>
        <translation>If checked :</translation>
    </message>
    <message>
        <source>Chaque item sera exporté dans un fichier différent
Le nom du fichier à utiliser sera fonction du choix que vous allez faire ci-dessous.</source>
        <translation>Each item will be exported to a different file
The name of the file to be used will depend on the choice you make below.</translation>
    </message>
    <message>
        <source>Exporter un item par fichier</source>
        <translation>Exporting an item by file</translation>
    </message>
    <message>
        <source>Si coché : 
L&apos;attribut à utiliser appartiendra à l&apos;item à exporter</source>
        <translation>If checked :
The attribute to be used will belong to the item to be exported</translation>
    </message>
    <message>
        <source>Si coché : 
L&apos;attribut à utiliser appartiendra à un autre item que celui à exporter</source>
        <translation>If checked :
The attribute to be used will belong to another item than the one to be exported</translation>
    </message>
    <message>
        <source>Préfixer le nom de fichier par le nom du tour courant (boucles uniquement)</source>
        <translation>Prefix the file name with the name of the current turn (loops only)</translation>
    </message>
    <message>
        <source>Si coché, comment déterminer le nom du fichier ?</source>
        <translation>If checked, how to determine the file name?</translation>
    </message>
    <message>
        <source>Un fichier sera créé pour chaque item. Comment déterminer le nom du fichier ?</source>
        <translation>A file will be created for each item. How to determine the name of the file?</translation>
    </message>
    <message>
        <source>Choisir un nom du fichier</source>
        <translation>Choose a file name</translation>
    </message>
    <message>
        <source>Saisie manuelle d&apos;un nom de base.
Chaque item génére un fichier nommé NomDeBase_xx, avec xx allant de 0 à n. </source>
        <translation>Manual entry of a base name.
Each item generates a file named BaseName_xx, with xx ranging from 0 to n.</translation>
    </message>
    <message>
        <source>Nom du fichier contenu dans un attribut</source>
        <translation>Name of the file contained in an attribute</translation>
    </message>
    <message>
        <source>Nom du fichier contenu dans un attribut d&apos;un autre item</source>
        <translation>Name of the file contained in an attribute of another item</translation>
    </message>
</context>
<context>
    <name>PB_StepGenericLoadFile</name>
    <message>
        <source>Charge un fichier du type</source>
        <translation>Load a file of type</translation>
    </message>
    <message>
        <source>%1 : %2</source>
        <translation>%1 : %2</translation>
    </message>
</context>
<context>
    <name>PB_StepLoadFileByName</name>
    <message>
        <source>Choisir un fichier exemple</source>
        <translation>Choose a sample file</translation>
    </message>
    <message>
        <source>Le fichier choisi doit :</source>
        <translation>Choosen file must:</translation>
    </message>
    <message>
        <source>- Etre dans le répertoire des fichiers à charger</source>
        <translation>- Be in the directory of the files to be loaded</translation>
    </message>
    <message>
        <source>- Avoir le même format que les fichiers à charger</source>
        <translation>- Have the same format as the files to be loaded</translation>
    </message>
    <message>
        <source>- Avoir la même structure / version que les fichiers à charger</source>
        <translation>- Have the same structure / version as the files to be loaded</translation>
    </message>
    <message>
        <source>Résultat</source>
        <translation>Result</translation>
    </message>
    <message>
        <source>Item</source>
        <translation>Item</translation>
    </message>
    <message>
        <source>Nom</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Chargement du fichier %1</source>
        <translation>Loading file %1</translation>
    </message>
    <message>
        <source>Fichier %1 inexistant ou non valide</source>
        <translation>Fichier %1 not existing or not valid</translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Entête</source>
        <translation>Header</translation>
    </message>
    <message>
        <source>Créer un sélecteur de fichier par attribut de nom</source>
        <translation>Create a file selector by name attribute</translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation>No reader selected</translation>
    </message>
    <message>
        <source>Reader</source>
        <translation>Reader</translation>
    </message>
    <message>
        <source>Cette étape charge l&apos;entête d&apos;un fichier, dont le nom est déterminé par un attribut choisi.&lt;br&gt;L&apos;utilisateur doit choisir un fichier exemple, qui est utilisé pour définir le répertoire de recherche et vérifier le format utilisé.</source>
        <translation>This step loads a file header, the name of which is determined by a chosen attribute.&lt;br&gt;The user must choose an example file, which is used to define the search directory and check the format used.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;L&apos;attibut sélectionné doit contenir le nom de base du fichier recherché. Si l&apos;attribut contient un chemin complet, cette étape en extraiera uniquement le nom de base (sans chemin d&apos;accès et sans extension).&lt;br&gt;Il s&apos;agit souvent d&apos;un attribut nommé &quot;filename&quot; ou &quot;filepath&quot;</source>
        <translation>&lt;br&gt;&lt;br&gt;The selected attribute must contain the base name of the file being searched. If the attribute contains a full path, this step will extract only the base name (without path and extension).&lt;br&gt;This is often an attribute named &quot;filename&quot; or &quot;filepath&quot;</translation>
    </message>
    <message>
        <source>&lt;br&gt;Cette étape charge uniquement l&apos;entête du fichier. &lt;strong&gt;Il faudra ensuite charger les données du fichier&lt;/strong&gt;, en général à l&apos;aide de l&apos;étape &quot;Charger les fichiers d&apos;une liste&quot;.</source>
        <translation>&lt;br&gt;This step only loads the header of the file. &lt;strong&gt;Then you will need to load the data from the file&lt;/strong&gt;, usually using the &quot;Load files from a list&quot; step.</translation>
    </message>
    <message>
        <source>En général, cette étape est utilisée dans une boucle, afin de charger un fichier correspondant au tour de boucle en cours.&lt;br&gt;Par exemple, si on a une boucle qui charge tour à tour des nuages de points LIDAR, cette étape peut être utilisée pour identifier à chaque tour de boucle le fichier modèle numérique de terrain (MNT) ayant le même nom que le fichier LIDAR en cours, dans un répertoire dédié.</source>
        <translation>In general, this step is used in a loop, to load a file corresponding to the current loop turn.&lt;br&gt;For example, if one has a loop that loads LIDAR point clouds in turn, this step can be used to identify at each loop turn the Digital Terrain Model (DTM) file having the same name as the current LIDAR file, in a dedicated directory.</translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation>Choice of file type</translation>
    </message>
    <message>
        <source>Ce fichier exemple doit être l&apos;un des fichiers potentiels à charger (peu importe lequel). Il est analysé pour vérifier le format utilisé, mais aussi pour définir dans quel répertoire il faut chercher les fichiers. </source>
        <translation>This example file must be one of the potential files to be uploaded (it does not matter which one). It is analysed to check the format used, but also to define in which directory to look for the files.</translation>
    </message>
</context>
<context>
    <name>PB_StepLoopOnFileSets</name>
    <message>
        <source>Fichier texte (*.txt) ; Fichier texte (*.*)</source>
        <translation>Text file (*.txt) ; Text file (*.*)</translation>
    </message>
    <message>
        <source>Liste de readers</source>
        <translation>Readers list</translation>
    </message>
    <message>
        <source>Groupe</source>
        <translation>Group</translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Entête</source>
        <translation>Header</translation>
    </message>
    <message>
        <source>Boucle sur des groupes de fichiers</source>
        <translation>Loop over groups of files</translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation>Choice of file type</translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation>No reader selected</translation>
    </message>
    <message>
        <source>Fichier ou dossier non sélectionné</source>
        <translation>File or folder not selected</translation>
    </message>
    <message>
        <source>Aucun lot trouvé dans le fichier &quot;%1&quot;</source>
        <translation>No batches found in the &quot;%1&quot; file</translation>
    </message>
    <message>
        <source>Vérification des fichiers en cours...</source>
        <translation>Checking current files...</translation>
    </message>
    <message>
        <source>Annuler</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <source>Fichier &quot;%1&quot; non accepté par le reader</source>
        <translation>File &quot;%1&quot; not accepted by the reader</translation>
    </message>
    <message>
        <source>Reader</source>
        <translation>Reader</translation>
    </message>
    <message>
        <source>Ajout du fichier %1 du lot %2</source>
        <translation>Add file %1 from batch %2</translation>
    </message>
    <message>
        <source>Fichier %1 non valide</source>
        <translation>File %1 invalid</translation>
    </message>
    <message>
        <source>Chemin du fichier contenant en première colonne les noms des lots et en deuxième colonne les noms des fichiers à traiter (le fichier ne doit pas avoir d&apos;en-tête).</source>
        <translation>Path of the file containing in the first column the names of the batches and in the second column the names of the files to be processed (the file must not have a header).</translation>
    </message>
    <message>
        <source>Chemin du dossier contenant les fichiers à traiter.</source>
        <translation>Path of the folder containing the files to be processed.</translation>
    </message>
    <message>
        <source>Cette étape démarre une boucle de script. Pour cela elle utilise un fichier de correspondance entre des noms de groupes et des fichiers.A chaque tour de boucle, cette étape créée une liste des fichiers appartenant à l&apos;un des groupes.&lt;br&gt;Les groupes sont définis dans un fichier de paramétrage ASCII (sans ligne d&apos;entête) où :&lt;ul&gt;&lt;li&gt;La première colone indique le nom des groupes.&lt;/li&gt;&lt;li&gt;La seconde le chemin des fichiers correspondants.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>This step starts a script loop. For this it uses a file containing correspondence between group names and files.At each loop turn, this step creates a list of files belonging to one of the groups.&lt;br&gt;The groups are defined in an ASCII settings file (without a header line) where:&lt;ul&gt;&lt;li&gt;The first column indicates the name of the groups.&lt;/li&gt;&lt;li&gt;The second indicates the path of the corresponding files.&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Cette étape génère deux résultats :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.&lt;/li&gt;&lt;li&gt;Un résultat contenant le nom du groupe, ainsi que la liste des fichiers de ce groupe (non chargés à ce stade)&lt;./li&gt;&lt;/ul&gt;</source>
        <translation>&lt;br&gt;&lt;br&gt;This step generates two results:&lt;br&gt;&lt;ul&gt;&lt;li&gt;A counter result, allowing the management of the loop. In particular, this counter is used to find out the name of the current turn.&lt;/li&gt;&lt;li&gt;A result containing the name of the group, as well as the list of files of this group (not loaded at this stage)&lt;./li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>Cette étape est utile pour charger des séries de fichiers devant être traités ensembles.&lt;br&gt;Le nom de groupe pourra également être utilisé, par exemple pour charger un autre fichier lié au groupe, en utilisant l&apos;étape &quot;Créer un sélecteur de fichier par attribut de nom&quot;.&lt;br&gt;&lt;br&gt;Un exemple d&apos;utilisation : charger un nuage de point d&apos;une placette LIDAR, ainsi que les nuages de points des arbres de cette placette (préalablement segmentés).&lt;br&gt;Dans le fichier de correspondance on mettra&lt;br&gt;&lt;ul&gt;&lt;li&gt;Le nom dess placette en tant que noms de groupes en première colonne.&lt;/li&gt;&lt;li&gt;Le nom de chaque fichier arbre en tant que noms de fichiers en seconde colonne.&lt;/li&gt;&lt;/ul&gt;Cette étape créera la liste des fichiers pour chacune des placettes à chacun des tours de boucle. Ils pourront ensuite être chargés dans la boucle à l&apos;aide de l&apos;étape &quot;Charger les fichiers d&apos;une liste&quot;.&lt;br&gt;Ensuite, en utilisant l&apos;étape &quot;Créer un sélecteur de fichier par attribut de nom&quot;, suivie de l&apos;étape  &quot;Charger les fichiers d&apos;une liste&quot;, on pourra charger le fichier de la placette, pour peu que celui-ci soit nommé conformément au nom de placette utilisé comme nom de groupe. </source>
        <translation>This step is useful for loading series of files that need to be processed together.&lt;br&gt;The group name can also be used, for example, to load another file related to the group, using the step &quot;Create a file selector by name attribute&quot;.&lt;br&gt;&lt;br&gt;An example of use: loading a point cloud of a LIDAR plot, as well as the point clouds of the trees of this plot (previously segmented).&lt;br&gt;In the correspondence file we will put&lt;br&gt;&lt;ul&gt;&lt;li&gt;The plot name as group names in the first column.&lt;/li&gt;&lt;li&gt;The name of each tree file as file names in the second column.&lt;/li&gt;&lt;/ul&gt;This step will create the list of files for each of the plots at each loop turn. These can then be loaded into the loop using the &quot;Load files from a list&quot; step.&lt;br&gt;Then, using the &quot;Create a file selector by name attribute&quot; step, followed by the &quot;Load files from a list&quot; step, the plot file can be loaded, provided it is named in accordance with the plot name used as the group name.</translation>
    </message>
    <message>
        <source>Le type choisi doit correspondre aux fichiers listés dans le fichier de correspondance. </source>
        <translation>The type chosen must match the files listed in the correspondence file.</translation>
    </message>
    <message>
        <source>Choix du fichier de correspondance entre groupes et fichiers</source>
        <translation>Choice of file for correspondence between groups and files</translation>
    </message>
    <message>
        <source>La première colonne doit contenir le nom des groupes, la seconde colonne doit contenir le nom des fichiers du groupe (avec ou sans extension). Le fichier ne doit pas avoir de ligne d&apos;en-tête.</source>
        <translation>The first column should contain the name of the groups, the second column should contain the name of the files in the group (with or without extension). The file must not have a header line.</translation>
    </message>
    <message>
        <source>Choix du dossier contenant les fichiers listés dans le fichier de correspondance</source>
        <translation>Selection of the folder containing the files listed in the correspondence file</translation>
    </message>
    <message>
        <source>Groupes</source>
        <translation>Groups</translation>
    </message>
    <message>
        <source>Nom du Groupe</source>
        <translation>Name of the Group</translation>
    </message>
    <message>
        <source>Fichiers du groupe</source>
        <translation>Group files</translation>
    </message>
</context>
<context>
    <name>PB_StepLoopOnFiles</name>
    <message>
        <source>Liste de readers</source>
        <translation>Readers list</translation>
    </message>
    <message>
        <source>Erreur</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>Fichiers</source>
        <translation>Files</translation>
    </message>
    <message>
        <source>Entête</source>
        <translation>Header</translation>
    </message>
    <message>
        <source>Boucle sur les fichiers d&apos;un dossier</source>
        <translation>Loop over files in a folder</translation>
    </message>
    <message>
        <source>Choix du type de fichier</source>
        <translation>Choice of file type</translation>
    </message>
    <message>
        <source>Aucun reader sélectionné</source>
        <translation>No reader selected</translation>
    </message>
    <message>
        <source>Choix du répertoire contenant les fichiers</source>
        <translation>Choice of the directory containing the files</translation>
    </message>
    <message>
        <source>Dossier vide</source>
        <translation>Empty folder</translation>
    </message>
    <message>
        <source>Aucun fichier compatible dans le dossier</source>
        <translation>No compatible files in the folder</translation>
    </message>
    <message>
        <source>Vérification des fichiers en cours...</source>
        <translation>Checking current files...</translation>
    </message>
    <message>
        <source>Annuler</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <source>Fichier &quot;%1&quot; non accepté par le reader</source>
        <translation>File &quot;%1&quot; not accepted by the reader</translation>
    </message>
    <message>
        <source>Reader</source>
        <translation>Reader</translation>
    </message>
    <message>
        <source>Ajout du fichier %1</source>
        <translation>Add file %1</translation>
    </message>
    <message>
        <source>Fichier %1 non valide</source>
        <translation>File %1 invalid</translation>
    </message>
    <message>
        <source>Chemin du dossier contenant les fichiers à traiter.</source>
        <translation>Path of the folder containing the files to be processed.</translation>
    </message>
    <message>
        <source>Cette étape permet d&apos;effectuer des traitements par lots.&lt;br&gt;Elle créée une boucle permettant de traiter successivement les fichiers choisis.&lt;br&gt;IMPORTANT : la boucle devra être fermée avec l&apos;étape &quot;Fin de boucle&quot;.&lt;br&gt;&lt;br&gt;Cette étape est en fait une combinaison des étapes &quot;Créer une liste de fichiers&quot; et &quot;Boucle standard&quot;. </source>
        <translation>This step allows batch processing.&lt;br&gt;It creates a loop to successively process the selected files.&lt;br&gt;IMPORTANT: the loop will have to be closed with the &quot;End of loop&quot; step.&lt;br&gt;&lt;br&gt;This step is actually a combination of the &quot;Create a file list&quot; and &quot;Standard loop&quot; steps.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Cette étape génère deux résultats :&lt;br&gt;&lt;ul&gt;&lt;li&gt;Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.&lt;/li&gt;&lt;li&gt;Un résultat contenant l&apos;entête du fichier correspondant à ce tour de boucle. &lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;br&gt;&lt;br&gt;This step generates two results:&lt;br&gt;&lt;ul&gt;&lt;li&gt;A result Counter, allowing the management of the loop. In particular, this counter is used to know the name of the current loop turn.&lt;/li&gt;&lt;li&gt;A result containing the header of the file corresponding to this loop turn. &lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>On préfèrera la combinaison des étapes &quot;Créer une liste de fichiers&quot; et &quot;Boucle standard&quot;, dans les cas où la liste complète des fichiers à charger est également utile dans le script, par exemple s&apos;il faut générer initialement l&apos;ensemble des emprises des fichiers à charger.&lt;br&gt;A l&apos;inverse cette étape est plus simple s&apos;il suffit de parcourir une liste de fichiers séquenciellement. </source>
        <translation>The combination of the &quot;Create File List&quot; and &quot;Standard Loop&quot; steps is preferred in cases where the complete list of files to be uploaded is also useful in the script, for example if the entire set of extents of the files to be uploaded must be generated initially.&lt;br&gt;Inversely, this step is simpler if it is sufficient to go through a list of files sequentially.</translation>
    </message>
</context>
<context>
    <name>PB_StepUseReaderToLoadFiles</name>
    <message>
        <source>Fichiers</source>
        <translation>Files</translation>
    </message>
    <message>
        <source>Chargement du fichier %1</source>
        <translation>Loading file %1</translation>
    </message>
    <message>
        <source>Fichier</source>
        <translation>File</translation>
    </message>
    <message>
        <source>Charger les fichiers d&apos;une liste</source>
        <translation>Loading files from a list</translation>
    </message>
    <message>
        <source>Reader</source>
        <translation>Reader</translation>
    </message>
    <message>
        <source>Cette étape permet de charger effectivement un fichier, dont l&apos;entête a préalablement été créée, par exemple avec l&apos;étape &quot;Créer une liste de fichiers&quot;.</source>
        <translation>This step allows you to actually load a file, whose header has been created before, for example with the step &quot;Create a file list&quot;.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;L&apos;item choisi définit sur quel fichier il faut charger. Ces fichiers doivent avoir été préalable listés. C&apos;est l&apos;objet &quot;reader&quot; qui est utilisé pour réaliser le chargement.</source>
        <translation>&lt;br&gt;&lt;br&gt;The chosen item defines which file to load. These files must have been previously listed. It is the &quot;reader&quot; object that is used to perform the loading.</translation>
    </message>
    <message>
        <source>Cette étape charge les données du fichier. La structure de données dépend du format du fichier.</source>
        <translation>This step loads the data from the file. The data structure depends on the file format.</translation>
    </message>
    <message>
        <source>L&apos;utilisation la plus habituelle de charger un fichier par tour de boucle, à partie d&apos;une liste de fichiers préalablement crée. Dans ce cas, la structure du script est la suivante :&lt;ol&gt;&lt;li&gt;Créer une liste de fichier (séléction de la liste des fichiers à parcourir)&lt;/li&gt;&lt;li&gt;Boucle standard (début de la boucle)&lt;/li&gt;&lt;li&gt;&lt;strong&gt;Charger les fichiers d&apos;une liste&lt;/strong&gt; (chargement du fichier correspondant au tour de boucle courant)&lt;/li&gt;&lt;li&gt;... (étapes de traitement et d&apos;export)&lt;/li&gt;&lt;li&gt;Fin de boucle&lt;/li&gt;&lt;/ol&gt;&lt;br&gt;</source>
        <translation>The most usual use of loading a file per loop turn, from a previously created file list. In this case, the structure of the script is as follows:&lt;ol&gt;&lt;li&gt;Create file list (select list of files to browse)&lt;/li&gt;&lt;li&gt;Standard loop (start of loop)&lt;/li&gt;&lt;li&gt;&lt;strong&gt;Load files from a list&lt;/strong&gt; (load file corresponding to the current loop turn)&lt;/li&gt;&lt;li&gt;... (processing and export steps)&lt;/li&gt;&lt;li&gt;End of loop&lt;/li&gt;&lt;/ol&gt;&lt;br&gt;</translation>
    </message>
</context>
<context>
    <name>PB_StepUserItemSelection</name>
    <message>
        <source>Mode manuel</source>
        <translation>Manual mode</translation>
    </message>
    <message>
        <source>Bienvenue dans le mode manuel de cette étape de filtrage. Veuillez sélectionner les éléments dans la vue graphique puis valider en cliquant sur le pouce en haut de la fenêtre principale. Les éléments sélectionnés seront gardés dans le résultat de sortie.</source>
        <translation>Welcome to the manual mode of this filtering step. Please select the items in the graphical view and confirm by clicking on the thumb on top of the main window. Selected items will be kept in the output result.</translation>
    </message>
    <message>
        <source>Item</source>
        <translation>Item</translation>
    </message>
    <message>
        <source>Séléction interactive d&apos;items</source>
        <translation>Interactive selection of items</translation>
    </message>
    <message>
        <source>Cette étape permet à l&apos;utilisateur de faire une séléction manuelle interactive des items à conserver. </source>
        <translation>This step allows the user to make an interactive manual selection of items to be retained.</translation>
    </message>
    <message>
        <source>&lt;br&gt;&lt;br&gt;Il faut simplement choisir les items parmi lesquels faire la séléction interactive. </source>
        <translation>&lt;br&gt;&lt;br&gt;You simply choose the items from which to make the interactive selection.</translation>
    </message>
    <message>
        <source>Cette étape génère une copie du résultat d&apos;entrée, mais où seuls les items sélectionnés sont conservés. Le reste des données n&apos;est pas impacté. </source>
        <translation>This step generates a copy of the input result, but only the selected items are retained. The rest of the data is not affected.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Plugin_Base initialized</source>
        <translation>Plugin_Base initialized</translation>
    </message>
</context>
</TS>
