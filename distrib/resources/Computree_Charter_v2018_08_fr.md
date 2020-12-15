<table>
<tbody>
<tr class="odd">
<td><img src="/attachments/download/18/en_US.png" alt="" /> [[En_wiki_v6_Charter</td>
<td>..english version of this page]]</td>
</tr>
</tbody>
</table>

# CHARTE COMPUTREE

**Version pdf de cette charte** :
attachment:Charte\_Computree\_v2018\_08.pdf

 

{{toc}}

 

*La présente charte définit les modalités de fonctionnement de la
plateforme Computree. Elle décrit les objectifs de la plateforme, les
règles de comportement, les différents rôles au sein de la communauté
Computree, ainsi que les modalités de gouvernance et de gestion de la
plateforme.*

**L’utilisation de la plateforme Computree, ou le développement de
plugins et de librairies Computree implique l’acceptation expresse des
conditions de la présente Charte.**

**Le non-respect des dispositions de la présente charte est susceptible
d’entraîner l’exclusion de la Communauté Computree.**

 

# PREAMBULE : PRÉSENTATION DE LA PLATEFORME

## 1\. Historique

La plateforme Computree a été initiée au printemps 2010 dans le cadre
d’une collaboration entre le département Recherche, Développement et
Innovation (RDI) de l’Office national des forêts (ONF) et l’Ecole
nationale supérieure des arts et métiers (ENSAM) de Cluny, au sein du
projet ANR Emerge. Entre 2010 et 2016 le cœur de la plateforme a été
développé et maintenu sur financements majoritairement ONF, complétés
par des contributions des projets ANR Emerge et Foresee. Les
développements en eux-mêmes ont été réalisés par l’ONF (essentiellement
algorithmes de traitement), et par l’ENSAM dans le cadre de contrats de
recherche (principalement cœur de la plateforme). Initialement centrée
sur le traitement des données LIDAR terrestre, la plateforme a été
ouverte aux autres sources de données telles que le LIDAR aérien à
partir de 2014.

En 2018, s’est organisé le transfert de la gouvernance et de la gestion
de la plateforme de l’ONF vers le groupement d’intérêt public Ecofor qui
a constitué un **groupe Computree**. Ce dernier est composé des
organismes ou établissements allouant des ressources dédiées à la
plateforme dans la durée (temps de personnel, contributions matérielles
ou logistiques, apport financier). La partie « Modalités de gouvernance,
de gestion et de fonctionnement » de cette charte décrit cet aspect de
façon approfondie.

Ce transfert de gouvernance est motivé par les objectifs suivants :

  - Ancrer la philosophie collaborative de Computree dans son mode de
    gouvernance.
  - Mutualiser son financement, pour faciliter et sécuriser son
    évolution et sa maintenance.
  - Appuyer la communauté Computree par la constitution progressive
    d’une équipe projet multipartenaires (en particulier par le biais
    des contributions en temps de personnel).
  - Assurer un équilibre entre objectifs scientifiques et opérationnels
    par la diversité et la complémentarité des partenaires du groupe.
  - Favoriser l’ouverture internationale de la plateforme. 

Les membres fondateurs du groupe Computree sont : **l’Office National
des Forêts, le GIP Ecofor, l’Institut National de l’Information
Géographique et Forestière, l’Institut National de la Recherche
Agronomique et l’Université de Sherbrooke**.

## 2\. Objectifs de la plateforme

La plateforme Computree est un outil collaboratif de traitement de
données dans des contextes forestiers ou de gestion des milieux
naturels en général. Elle propose un environnement modulaire permettant
de traiter principalement des données de télédétection : LIDAR
terrestre, LIDAR aérien, photographies aériennes ou satellites, données
rasters (modèles numériques de terrain, de surface ou de hauteur),…
Cependant ses capacités de traitement peuvent être étendues à tout type
de donnée géométrique 3D, 2D, ou attributaire.

L’objectif principal de Computree est de favoriser les synergies dans le
développement et l’utilisation de méthodes de traitement des données, à
des fins de description, d’analyse et d’inventaire de peuplements
forestiers ou autres milieux naturels. Cet objectif se décline sous
trois angles de vue complémentaires :

  - Faciliter le développement, la validation, la mise à disposition et
    l’analyse comparative (benchmarking) d’algorithmes et de chaînes de
    traitement (workflows) innovants.
  - Permettre une utilisation simple et performante de ces algorithmes à
    des fins de recherche scientifique ; dans ce cas la plateforme
    Computree est un outil d’analyse utilisé pour répondre à des
    questions scientifiques liées à la forêt. 
  - Faire monter en maturité les méthodes et les outils, et les rendre
    disponibles in fine pour un usage opérationnel en production, dans
    tout domaine pertinent, tel que l’inventaire forestier, la
    description des milieux naturels, la planification ou la gestion
    forestière. 

L’interaction entre ces trois visions est au cœur de la philosophie de
la plateforme Computree.

Les fonctionnalités de la plateforme peuvent être étendues à des usages
sans rapport avec les milieux naturels, via des plugins. Cependant, les
ressources partagées allouées à la gestion et la maintenance de la
plateforme n’interviendront pas spécifiquement dans ces domaines.

## 3\. Architecture de la plateforme

La plateforme est construite selon une architecture modulaire facilitant
l’ajout de fonctionnalités par des équipes indépendantes de la
communauté Computree. Ainsi, elle est composée des éléments suivants :

  - Le **cœur** contient toutes les fonctionnalités communes de gestion
    des traitements, les structures de données partagées, ainsi que le
    système de gestion des plugins. Le cœur est également en charge de
    la connexion avec les librairies externes utilisables au sein de la
    plateforme.
  - L’ **interface graphique standard** permet l’utilisation interactive
    de toutes les fonctionnalités du cœur et de ses extensions
    (plugins).
  - L’ **interface batch standard** permet l’exécution, sans interface
    graphique, de scripts de traitement Computree. 
  - Les **plugins** sont des modules autonomes, connectés uniquement au
    cœur, permettant d’ajouter des fonctionnalités. Le développement
    d’un plugin est totalement indépendant de celui des autres
    plugins. Cependant, chaque plugin peut être utilisé en conjonction
    avec d’autres plugins lors de l’exécution.
  - Les **librairies** sont des modules autonomes, connectés uniquement
    au cœur, mettant des fonctionnalités génériques à disposition des
    plugins. 

Un plugin et plusieurs librairies de base sont fournies en standard avec
la plateforme, et contiennent les fonctionnalités essentielles à la
création de chaînes de traitement (en particulier formats d’import /
export).

Il n’y pas de limite au nombre de plugins ou de librairies que l’on peut
ajouter à Computree. De la même façon, le système est conçu pour
permettre l’utilisation de nouvelles interfaces, graphiques ou non,
développées par exemple pour des usages spécifiques valorisant certains
plugins uniquement.

Le principe de la plateforme est de proposer, par le biais des
interfaces, la possibilité pour l’utilisateur de créer des séquences de
traitement (workflows), à partir de briques unitaires nommées « étapes »
contenues dans les plugins. Chaque étape permet à partir de données
d’entrée, d’appliquer un algorithme pour produire des données de
sortie. Ainsi, en enchaînant des étapes unitaires, potentiellement
issues de différents plugins, l’utilisateur peut constituer un
traitement complet adapté à ses besoins. Certaines étapes peuvent être
très génériques (donc utilisables dans de nombreux contextes). D’autres
sont dédiées à un usage très spécifique.

La séquence de traitement peut être intégralement constituée et
sauvegardée sous forme de script, avant exécution immédiate ou décalée.

 

# I. DÉFINITIONS

On entend par « **Communauté Computree** » l’ensemble des personnes
physiques ou morales appelées à développer ou utiliser la plateforme
Computree. Afin de clarifier les interactions au sein de la communauté
Computree, la présente charte définit plusieurs « rôles Computree ». Une
même personne peut avoir plusieurs rôles au même moment, ou dans le
temps :

  - **Responsable du groupe Computree** : personne morale administrant
    la communauté Computree pour le compte des membres du groupe et de
    leur comité stratégique ; en 2018, l’ONF transfère cette
    responsabilité au GIP Ecofor par voie de convention. Ultérieurement,
    cette responsabilité pourra être transférée si nécessaire à un autre
    membre du groupe par le comité stratégique (voir article III.1 de la
    présente charte).
  - **Membre du groupe Computree** : personne morale ou physique sous
    convention avec le Responsable du groupe Computree pour une
    contribution aux ressources allouées à la plateforme Computree, et
    ayant un droit de vote dans la définition de la stratégie de
    maintenance et d’évolution de la plateforme.
  - **Gestionnaire Computree** : personne chargée d’appliquer la
    stratégie de maintenance et d’évolution de la plateforme. Elle
    anime la communauté Computree et organise les travaux de
    développement. Cette personne est responsable du comité technique,
    a le rôle de chef de projet des développements informatiques et rend
    compte au comité stratégique. Le gestionnaire Computree constitue le
    contact pour toute question venant de l’extérieur. Ce rôle est
    attribué chaque année, par le comité stratégique (voir article III.1
    de la présente charte), à l’une des personnes ayant du temps de
    travail alloué à la plateforme. 
  - **Développeur Computree** : personne chargée de la maintenance
    applicative et de l’implémentation de nouveaux éléments centraux de
    la plateforme (voir article II). L’équipe des développeurs Computree
    est constituée de personnes dont tout ou partie du temps de travail
    a été alloué à la plateforme par les membres du groupe Computree.
    Elle comprend aussi les développeurs financés contractuellement par
    le Responsable du groupe Computree à l’aide des ressources
    financières allouées à la plateforme. Cette équipe est pilotée par
    le gestionnaire Computree. 
  - **Animateur Computree** : personne chargée de réaliser des
    séminaires « utilisateur » ou « développeur de plugin » Computree.
    L’équipe des animateurs Computree est constituée de personnes dont
    tout ou partie du temps de travail a été alloué à la plateforme par
    les membres du groupe Computree. Cette équipe est pilotée par le
    gestionnaire Computree.
  - **Utilisateur Computree** : personne utilisant la plateforme
    Computree et ses plugins pour des applications scientifiques ou non.
    
  - **Propriétaire de plugin Computree** : personne morale ou physique
    détentrice des droits sur un plugin ; elle en gère le développement
    et la diffusion.
  - **Développeur de plugin Computree** : personne chargée de la
    maintenance et de l’implémentation de nouvelles fonctionnalités dans
    un plugin spécifique.

 

# II. UTILISATION DE LA PLATEFORME ET DROIT DE PROPRIÉTÉ INTELLECTUELLE

Le cœur, l’interface Batch standard ainsi que les plugins communs et les
librairies communes sont diffusés sous licence open-source LGPL (Lesser
General Public License). L’interface graphique standard est mise à
disposition sous licence GPL (General Public License).  
Ces éléments peuvent ainsi être utilisés pour tout usage professionnel
ou personnel, commercial ou non, respectant les conditions de la
présente Charte. Comme indiqué dans les licences LGPL et GPL, la
plateforme est distribuée sans aucune garantie, ni explicite ni
implicite.

warning\<\>. **Il est demandé aux utilisateurs de Computree de citer
explicitement la plateforme, les plugins et les librairies utilisés dans
tous les travaux publiés, valorisant ainsi les résultats obtenus à
l’aide de Computree. La plateforme fournit à ce titre une
fonctionnalité permettant d’obtenir les informations de citation
appropriées à chaque chaine de traitement créée par l’utilisateur.**

Chaque plugin ou librairie (en dehors des plugins communs) est la
propriété d’un auteur, d’un groupe, d’un organisme, d’un établissement,
d’une société ou d’un membre de la communauté Computree qui a généré ce
plugin ou librairie. Le propriétaire du plugin a toute latitude pour
choisir les modalités de diffusion et les conditions d’utilisation de
son plugin. L’interface graphique standard de Computree étant sous
licence GPL, un plugin sous licence non compatible GPL devra soit rester
en usage interne, soit proposer une interface alternative. A noter que
l’interface Computree Batch n’impose aucune restriction de ce type.

La plateforme accueille donc plusieurs composantes (cœur, interfaces,
plugins, librairies) de différentes natures au regard des droits de
propriété intellectuelle et des droits d’utilisation qui leur sont
attachés.

**Les éléments centraux de la plateforme font l’objet d’une propriété
partagée entre les membres actuels et passés du groupe au prorata de
leur contribution totale quelle que soit leur nature (montant financier,
temps de personnel mis à disposition, ressources matérielles mises à
disposition, évalués financièrement) cumulée depuis le 1er janvier
2010**. Elles comprennent :

  - Le cœur,
  - L’interface graphique standard,
  - L’interface batch standard,
  - Des plugins communs.
  - Des librairies communes. 

L’ajout de nouveaux éléments (plugins et librairies communs) à la liste
des éléments centraux ne peut être fait que par le comité stratégique,
dans la mesure où ces éléments sont développés sur les ressources
allouées à la plateforme. Ils font l’objet d’une propriété partagée,
étant précisé que chaque membre reste titulaire des droits sur ses
apports initiaux (intégration de travaux antérieurs). Une liste des
éléments centraux est tenue à jour par le comité stratégique et est
incluse dans le rapport d’activité annuel. Est également inclus dans ce
rapport d’activité un état actualisé des contributions cumulées depuis
le 1er janvier 2010, de chacun de ses membres actifs ou ayant quitté le
groupe. Les contributions constatées de l’année en cours sont ajoutées
au bilan, après vote à l’unanimité de leur exactitude par le comité
stratégique. Ce bilan donne le détail annuel des contributions et de
leur nature pour chaque membre faisant ou ayant fait partie du groupe.

**Quant aux autres plugins ou librairies (cas général), la propriété
intellectuelle en est gérée de façon autonome** : l’appartenance au
groupe Computree ne donne aucun droit sur les plugins ou librairies ne
faisant pas partie de la liste des éléments centraux.

 

# III. MODALITÉS DE GOUVERNANCE, DE GESTION ET DE FONCTIONNEMENT

## 1\. Pilotage

Deux comités sont institués pour assurer la gestion de la Plateforme et
se réunissent une fois par an chacun.

Un **Comité Technique**, uniquement consultatif, a pour objectif de
faire des propositions d’évolution de la plateforme pour alimenter le
comité stratégique. Ce comité est animé par le gestionnaire Computree et
se réunit au moins une fois par an. Tous les membres du groupe Computree
sont invités à participer, et il est également ouvert à tout développeur
de plugin Computree. Le gestionnaire Computree fait un bilan des
évolutions de la plateforme sur l’année écoulée. Il fait également une
synthèse des demandes des utilisateurs obtenues par différents canaux,
dont en particulier le forum utilisateur Computree. Enfin les membres du
comité technique listent les propositions d’évolutions de la plateforme
pour l’année à venir et proposent des critères de priorisation.

Un **Comité Stratégique**, composé des membres du groupe Computree tel
que définis à l’article I, se réunit au moins une fois par an. Il prend
toutes les décisions concernant le groupe Computree et la gestion de ses
ressources. En particulier :

  - Il détermine (vote à la majorité absolue) les orientations générales
    de la Plateforme, les grandes priorités d’allocation des ressources
    pour l’année suivante, sur la base des propositions du comité
    technique, ou la réallocation éventuelle de ces ressources en cours
    d’année. 
  - Valide chaque année (vote à la majorité absolue) la version mise à
    jour du bilan des contributions cumulées depuis 2010 des membres
    actifs et passés, ainsi que de la liste des éléments centraux. Ces
    deux documents seront intégrés systématiquement au rapport annuel
    d’activité. 
  - Désigne le gestionnaire Computree pour l’année à venir (vote à la
    majorité absolue), qui peut être préférentiellement un agent des
    membres du groupe ou éventuellement un agent sous contrat avec le
    Responsable du groupe Computree. 
  - Chaque année, reconduit – ou transfert à un autre membre si
    nécessaire – la fonction de Responsable du groupe Computree (vote à
    l’unanimité). 
  - Statue sur l’intégration de nouveaux membres (vote à la majorité des
    deux tiers) selon la procédure décrite à l’article IV.1. 
  - Peux décider de l’éviction d’un membre ne respectant pas la présente
    Charte (vote à l’unanimité, le membre concerné n’étant pas pris en
    compte dans le vote), selon la procédure décrite à l’article IV.2.
  - Valide toute modification de la présente charte (vote à
    l’unanimité). Ces modifications devront faire l’objet d’un
    avenant signé par les membres du groupe.

## 2\. Droit de vote au comité stratégique

Seuls les membres du Groupe COMPUTREE disposent d’un droit de vote.

Le droit de vote au comité stratégique est déterminé en fonction de
l’ensemble des contributions apportées par le membre (montant
financier, temps de personnel mis à disposition, ressources matérielles
mises à disposition évalués financièrement), pour l’année en cours. Une
voix est attribuée par tranche complète de 1000 euros. Par exception, un
membre ne pourra pas se voir attribuer plus de 45 % du total des droits
de vote quel que soit le montant de sa contribution.

Des conventions bipartites entre le Responsable du groupe Computree et
les différents partenaires, fixent le niveau prévisionnel et la durée
(pour les contributions en temps de personnel) de ces contributions.

## 3\. Gestion administrative

La gestion administrative du groupe Computree est assurée par le
Responsable du groupe Computree. Sa mission consiste à :

  - Gérer le budget de la plateforme, étant précisé qu’une part
    significative des ressources est dédiée à la maintenance de la
    plateforme (évolutions systèmes, hébergement, correction de bugs,
    diffusion, documentation…) et au développement de certains
    composants jugés d’intérêt collectif par le comité stratégique.
  - Gérer les conventions d’adhésion au groupe et le respect de ses
    dispositions.
  - Gérer les conventions de sous-traitance de développement.
  - Planifier les réunions de gestion de la plateforme.
  - Administrer les ressources allouées.
  - Veiller au respect des dispositions de la présente Charte.
  - Veiller au bon fonctionnement de la Plateforme au quotidien.

## 4\. Allocation des ressources

Les ressources apportées par les membres du groupe (et les éventuelles
participations aux frais de séminaires demandées aux participants,
décrites à l’article III.5) sont utilisées pour la maintenance et les
évolutions des éléments centraux de Computree, les tests ainsi que pour
l’animation de la communauté Computree, la diffusion et la documentation
de la plateforme.

A noter que le développement des plugins et librairies n’est pas financé
sur les ressources allouées à la plateforme, à l’exception des plugins
et librairies inscrits à la liste des éléments centraux, dont le
développement aura été validé par le comité stratégique et la liste
tenue à jour par le gestionnaire Computree.

L’utilisation de Computree et les ressources mises à disposition pour
permettre et faciliter le développement de plugins sont entièrement
gratuits. Des ressources en ligne sont mises à disposition pour
faciliter l’autonomie des membres de la communauté Computree. Un forum
d’échange vise également à favoriser l’entraide au sein de la
communauté.

## 5\. Organisation de séminaires

Des séminaires « développeurs de plugins » sont proposés en fonction des
besoins. Ils sont gratuits pour tout plugin diffusé sous licence
Open-Source et restant dans les champs d’application des milieux
naturels ou de la forêt. Dans tous les autres cas, ils font l’objet d’un
paiement sur facture de frais de séminaires.

Chaque année, au moins un séminaire « utilisateurs Computree » est
proposé à la communauté, éventuellement plusieurs selon la demande et
les besoins. Ces séminaires ont pour objet d’initier les utilisateurs à
l’utilisation de la plateforme Computree. Des séminaires
d’approfondissement thématiques pourront également être proposés. Ces
séminaires pourront faire l’objet d’un paiement sur facture de frais de
séminaires occasionnés par chaque stagiaire. Afin d’en faciliter
l’accès, des séminaires « utilisateurs Computree » peuvent également
être proposés au catalogue de formation d’un ou plusieurs organismes
membres du groupe.

Les ressources éventuellement issues de l’organisation de ces séminaires
viennent abonder le budget permettant l’évolution de la plateforme. Par
ailleurs, les membres de la communauté Computree ont toute liberté pour
organiser leurs propres séminaires ou formations Computree.

 

# IV. EVOLUTION DE LA COMPOSITION DU GROUPE COMPUTREE

## 1\. Conditions d’admission et obligations des nouveaux membres

Lorsqu’un nouveau membre souhaite intégrer le groupe Computree, un
projet de convention avec le Responsable du groupe Computree –
spécifiant s’il y a lieu les contributions passées aux éléments
centraux de la plateforme, et dans tous les cas les contributions
prévues pendant la période de la convention – est rédigé en amont de la
réunion du comité stratégique. Au cours de la réunion, le comité
stratégique examine et statue sur l’intégration du nouveau membre à la
majorité des deux tiers. Une fois l’intégration validée et la convention
entre le nouveau membre et le Responsable du groupe Computree signée par
les deux parties, le nouveau membre intègre le groupe Computree et siège
au comité stratégique.

Les nouveaux membres s’engagent à respecter les obligations qui leur
incombent et les dispositions de la présente charte.

Les nouveaux membres s’engagent à allouer des ressources significatives
(de valeur équivalente supérieure ou égale à 5 000 €/ an), sur la durée,
pour maintenir ou développer les éléments centraux de la plateforme
Computree. Ces ressources sont définies dans le cadre d’une convention
pluriannuelle avec le Responsable du groupe Computree. Elles peuvent
être de différentes natures :

  - Montant financier.
  - Temps de personnel mis à disposition.
  - Ressources matérielles mises à disposition.

Dans tous les cas, ces ressources sont converties en un montant
monétaire équivalent, afin de déterminer les droits de vote de chaque
membre du groupe, au prorata de sa contribution pour l’année en cours.

## 2\. Conditions d’exclusion d’un membre

Dans le cas où l’un des membres manquerait aux obligations prévues dans
la présente charte, et après une mise en demeure du Responsable du
groupe Computree restée sans effet pendant un délai de trois mois, le
Comité stratégique peut décider de son éviction (par vote à l’unanimité,
le membre concerné n’étant pas pris en compte dans le vote), étant
précisé que les contributions versées ne lui sont pas restituées.

Dans ce cas le Comité Stratégique mandate le Responsable du groupe
Computree pour résilier la convention avec le membre concerné. La date
de résiliation sera fixée autant que possible en fin d’exercice annuel.

## 3\. Conditions de renouvellement pour les membres actifs

Lors de l’arrivée à échéance de la convention de l’un des membres du
groupe, ce membre peut de plein droit décider de son renouvellement (par
avenant ou nouvelle convention), sauf s’il a eu un comportement en
violation avec les dispositions de la présente charte.

Cependant, en cas d’augmentation de la contribution de plus de 50 %, le
comité stratégique doit donner son accord pour cette augmentation par un
vote à la majorité absolue, le membre concerné n’étant pas pris en
compte dans le vote.

Les droits de vote seront recalculés selon les modalités définies à
l’article III.2.

 

# V. RESPONSABILITES/GARANTIE

**La plateforme Computree est mise à disposition « en l’état », sans
garantie d’aucune sorte, explicite ou implicite, y compris, mais sans
limitation, les garanties implicites de qualité marchande et
d’adaptation à un usage en particulier. L’intégralité du risque de
qualité et de performance de la plateforme est supportée par
l’utilisateur. Si la plateforme est défectueuse, l’utilisateur assume
seul le coût de toute réparation nécessaire.**

Ainsi les membres du Groupe Computree ne sont pas responsables de
l’utilisation faite par les utilisateurs de la Plateforme ou des
résultats des analyses en découlant.

 

# VII. DISPOSITIONS GÉNÉRALES

Aucune stipulation de la présente Charte ne pourra être interprétée
comme constituant entre les les membres une entité juridique de quelque
nature que ce soit, ni impliquant une quelconque solidarité entre les
membres.

Les membres déclarent que la présente Charte ne peut en aucun cas être
interprétée ou considérée comme constituant un acte de société,
l’affectio societatis est formellement exclu.

Aucun membre n’a le pouvoir d’engager les autres membres, ni de créer
des obligations à la charge des autres membres, en dehors du Responsable
du groupe Computree dans le seul cadre de la mission qui lui est confiée
et dans la limite des droits qui lui sont conférés.

 

\[\[Fr\_wiki\_v6|Retour à l’accueil\]\]
