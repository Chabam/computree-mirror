<table>
<tbody>
<tr class="odd">
<td><img src="/attachments/download/19/fr_FR.png" alt="" /> [[Fr_wiki_v6_Charter</td>
<td>..version française de cette page]]</td>
</tr>
</tbody>
</table>

# COMPUTREE CHARTER

**PDF version of this charter**:
attachment:Computree\_Charter\_v2018\_08.pdf

 

{{toc}}

 

*This charter highlights main operating procedures of the Computree
platform. It describes the goals of the platform, the rules, the
different roles in the Computree community, as well as the governance
and management modalities of the platform.*

**The use of the Computree platform, or the development of Computree
plugins and libraries, implies the express acceptance of the provisions
of this Charter.**

**Failure to comply with the dispositions of this charter may result in
the exclusion of the Computree Community.**

 

# PREAMBLE: PRESENTATION OF THE PLATFORM

## 1\. History

The Computree platform started in spring 2010 as part of a collaboration
between the Research, Development and Innovation (RDI) department of the
Office National des Forêts (ONF) and the Ecole Nationale Supérieure des
Arts et Métiers (ENSAM) of Cluny, within the ANR Emerge project. Between
2010 and 2016, the core of the platform was developed and mostly
maintained through ONF budget, supplemented by contributions from the
ANR Emerge and Foresee projects. ONF (mainly processing algorithms), and
ENSAM research contracts (mainly core of the platform) shouldered most
of the programming. Initially focused on terrestrial LIDAR data
processing, the platform opened itself to other data sources such as the
airborne LIDAR since 2014.

In 2018, the governance and management of the platform was transferred
from ONF to the Computree group, under GIP ECOFOR. The group includes a
number of organizations or institutions contributing resources dedicated
to the platform (staff time, material or logistical contributions,
financial contribution). The “Terms of Governance, Management and
Operation” section of this charter describes this aspect in detail.

This transfer of governance is motivated by the following goals:

  - To carve Computree’s collaborative philosophy in its governance.
  - To mutualize its funding, to facilitate and secure its evolution and
    maintenance.
  - To support the Computree community through the gradual build-up of a
    multi-partner project team (especially as regards contributing staff
    allocations).
  - To ensure a balance between scientific and operational goals through
    the diversity and complementarity between group’s partners.
  - To promote international cooperation through the platform. 

The founding members of the Computree group are: **the Office National
des Forêts, the GIP Ecofor, the Institut National de l’Information
Géographique et Forestière, the Institut National de la Recherche
Agronomique and the University of Sherbrooke**.

## 2\. Objectives of the Platform

The Computree platform is a collaborative tool for data processing in
the context of forest management, or natural environment in general. It
offers a modular environment allowing mainly the treatment of remote
sensed data: terrestrial LIDAR, airborne LIDAR, airborne or satellite
photographs, raster data (digital terrain models, digital surface models
or canopy height models)… However its processing capacities can be
extended to any type of 3D geometric data, 2D, or attributes.

The main objective of Computree is to promote synergies in the
development and use of data processing methods in order to describe,
analyse and inventory forest stands or other natural environments. This
goal is further divided into three complementary sub-goals:

  - To facilitate the development, validation, provision and
    benchmarking of innovative algorithms and workflows.
  - To allow a simple and efficient use of these algorithms for
    scientific research purposes. In this case the Computree platform is
    an analysis tool used to answer scientific questions related to the
    forest.
  - To improve methods and tools, and ultimately make them available for
    operational use, in any relevant field, such as forest inventory,
    description of natural environments, forest planning or forest
    management. 

The linkages between those 3 dimensions is at the heart of the Computree
platform philosophy.

Though most features of the platform could accommodate other operating
environments, the shared resources allocated to the management and
maintenance of the platform will focus exclusively on natural
environments.

## 3\. Platform architecture

The platform is built on a modular architecture that makes it easy for
independent teams of the Computree community to add custom features The
platform encompasses the following elements:

  - The **core** contains all common process management features, shared
    data structures, and the plugin management system. The core is also
    in charge of the connection with external libraries, usable within
    the platform.
  - The **standard graphical interface** allows the interactive use of
    all the features of the core and its extensions (plugins).
  - The **standard batch interface** allows the execution, without
    graphical interface, of Computree processing scripts.
  - The\*plugins\* are autonomous modules, connected only to the core,
    used to add features. The development of a plugin is totally
    independent of the other plugins. However, each plugin can be used
    in conjunction with other plugins at runtime.
  - **Libraries** are standalone modules, connected only to the core,
    making generic features available to plugins. 

Several basic plugin and libraries are provided as standard with the
platform and contain the essential functionalities for the creation of
processing chains (in particular import/export formats).

There is no limit to the number of plugins or libraries that could be
added to Computree. The system is also designed to allow the use of new
interfaces, graphic or not, developed for example for specific uses or
certain plugins only.

The principle of the platform is to propose, through the interfaces, the
possibility for the user to create sequences of workflows, from bricks
named “steps” contained in the plugins. Each step consists of applying
an algorithm to input data in order to produce output data. Thus, by
linking steps, potentially from different plugins, the user can compose
a process adapted to his needs. Some steps can be very generic (thus
usable in many contexts). Others are dedicated to a very specific use.

The processing sequence can be fully constituted and saved as a script,
before immediate or delayed execution.

 

# I. DEFINITIONS

**“Computree Community”** means all physical or legal persons who
develop plugins or use the Computree platform. In order to clarify the
interactions within the Computree community, this charter defines
several “Computree roles”. The same person could have multiple roles at
the same time, or over time:

  - **Computree Group Leader**: a corporate entity administering the
    Computree Community on behalf of the Group Members and their
    Strategy Committee; in 2018, the ONF transfers this responsibility
    to the GIP Ecofor by agreement. Subsequently, this responsibility
    could be transferred if necessary to another member of the group by
    the strategic committee (see article III.1 of this charter).
  - **Member of the Computree group**: legal or physical person under
    agreement with the Computree Group Leader for a contribution to the
    resources allocated to the Computree platform, and having a voting
    right in the definition of the maintenance and evolution strategy of
    the platform.
  - **Computree Manager**: person responsible for delivering the
    maintenance and evolution strategy of the platform. She/he leads the
    Computree community and organizes development work. This person is
    responsible for the technical committee, has the role of development
    manager and reports to the strategic committee. The Computree
    Manager is the contact for any questions from third parties. This
    role is assigned each year, by the strategic committee (see Article
    III.1 of this charter), to one of the persons having working time
    allocated to the platform. 
  - **Computree developer**: person in charge of delivering maintenance
    and the production of new central elements of the platform (see
    article II). The Computree Developer Team is made up of people whose
    whole or part of the time has been allocated to the platform by
    members of the Computree Group. It also includes developers funded
    contractually by the Computree Group Leader using the financial
    resources allocated to the platform. This team is led by Computree
    Manager. 
  - **Computree facilitator**: person responsible for conducting
    Computree “user” or “plugin developer” seminars. The Computree
    facilitators’ team is made up of individuals whose whole or part of
    the time has been allocated to the platform by members of the
    Computree Group. This team is driven by Computree Manager.
  - **Computree user**: anyone using the Computree platform and its
    plugins for scientific or non-scientific applications. 
  - **Computree plugin owner**: legal or physical person holding the
    rights to a plugin, managing its development and dissemination.
  - **Computree plugin developer**: person in charge of the maintenance
    and the implementation of new functionalities within a specific
    plugin.

 

# II. USE OF THE PLATFORM AND INTELLECTUAL PROPERTY RIGHTS

The core, the standard batch interface as well as common plugins and
common libraries are released under the open-source LGPL (Lesser General
Public License). The standard graphical interface is made available
under the General Public License (GPL).  
These elements can thus be used for any professional or personal use,
commercial or not, respecting the conditions of this Charter. As stated
in the LGPL and GPL licenses, the platform is distributed without any
guarantee, neither explicit nor implicit.

warning\<\>. **Computree users are asked to explicitly cite the
platform, plugins and libraries used in all published works, valuing the
results obtained using Computree. As such, the platform provides a
functionality to obtain the appropriate citation information for each
user-created processing chain.**

Each plugin or library (apart from common plugins) is the property of an
author, group, organization, institution, company, or member of the
Computree community that has created this plugin or library. The owner
of the plugin has full latitude to choose the rules and modalities of
dissemination and conditions of use of its plugin. The Computree
standard graphical interface being under GPL license, a licensed non-GPL
compatible plugin will either have to remain in internal use, or offer
an alternative interface. Note that the Computree standard batch
interface does not impose any such restrictions.

The platform thus hosts several components (core, interfaces, plugins,
and libraries) of different nature with regard to the intellectual
property rights and the rights of use attached to them.

**The central elements of the platform are the subject of shared
ownership between the current and past members of the group in
proportion to their total contribution whatever their nature (financial
amount, staff time, material resources), evaluated financially and
cumulated since 1st January 2010. They include:**

  - The core,
  - The standard graphical interface,
  - The standard batch interface,
  - Common plugins.
  - Common librairies.

The addition of new elements (common plugins and libraries) to the list
of central elements can only be done by the strategic committee, insofar
as these elements are developed on the resources allocated to the
platform. They are subject to shared ownership, though each member
remains the owner of the rights on his initial contributions
(integration of previous works). A list of core elements is maintained
by the Strategic Committee and is included in the annual activity
report. Also included in this activity report is an updated statement of
contributions accumulated since January 1, 2010, of each of its active
or departing members. The contributions recognized for the current year
are added to the balance sheet, after unanimous vote of their accuracy
by the strategic committee. This report gives the annual details of the
contributions and their nature for each member who is or has been part
of the group.

**As for other plugins or libraries (general case), the intellectual
property is managed autonomously**: the membership in the Computree
group gives no rights on plugins or libraries that are not part of the
list of central elements.

 

# III. TERMS OF GOVERNANCE, MANAGEMENT AND OPERATION

## 1\. Management

Two committees are established to manage the Platform, and each meets
once a year.

A **Technical Committee**, only consultative, aims to make proposals for
the evolution of the platform to feed the strategic committee. This
committee is led by Computree Manager and meets at least once a year.
All members of the Computree group are welcome to participate, and it is
also open to any Computree plugin developer. The Computree Manager
reviews the evolution of the platform over the past year. It also
summarizes user requests from different channels, including the
Computree user forum. Finally, the members of the technical committee
list the proposals for changes to the platform for the coming year and
propose criteria for prioritization.

A **Strategy Committee**, composed of the members of the Computree Group
as defined in Article I, meets at least once a year. It makes all
decisions regarding the Computree Group and the management of its
resources. In particular:

  - It determines (absolute majority vote) the general orientations of
    the Platform, the main priorities for the allocation of resources
    for the following year, on the basis of the proposals of the
    technical committee, or the possible reallocation of these resources
    during the year.
  - It validates (absolute majority vote) the updated version of the
    balance sheet of contributions accumulated since 2010 of active and
    past members, as well as the updated list of central elements. These
    two documents will be systematically integrated into the annual
    activity report.
  - It appoints the Computree Manager for the coming year (absolute
    majority vote), which may be preferentially an employee of the group
    members or possibly an agent under contract with the Computree Group
    Leader.
  - It renews - or transfers to another member if necessary - the
    function of Computree Group Leader (vote by unanimity).
  - It decides on the integration of new members (two-thirds majority
    vote) according to the procedure described in Article IV.1.
  - It could decide to exclude a member who does not respect the present
    Charter (vote by unanimity, the member concerned not being taken
    into account in the vote), according to the procedure described in
    article IV.2.
  - It modifies this charter (vote by unanimity). These modifications
    must be the subject of an amendment signed by the members of the
    group.

## 2\. Voting right at the strategic committee

Only members of the Computree Group have a right to vote.

The weighting of votes in the strategic committee is proportional to all
the contributions made by the member (financial amount, staff time and
material resources evaluated financially), for the current year. One
vote is awarded per full part of 1000 euros. Exceptionally, a member may
not be awarded more than 45% of the total voting rights regardless of
the amount of his contribution.

Bipartite agreements between the Computree Group Leader and the various
partners set the expected level and duration (for staff time
contributions) of these contributions.

## 3\. Administrative management

The Computree Group administrative management is handled by the
Computree Group Leader. Its mission is to:

  - Manage the budget of the platform, noting a significant part of the
    resources is dedicated to the maintenance of the platform (system
    evolutions, hosting, bug fixing, dissemination, documentation …) and
    to the development of components selected by the Strategic
    Committee.
  - Manage group membership agreements and compliance with its
    provisions.
  - Manage development subcontracting agreements.
  - Plan the management meetings of the platform.
  - Administer allocated resources.
  - Ensure compliance with the provisions of this Charter.
  - Ensure daily the smooth running of the Platform.

## 4\. Allocation of resources

The resources provided by the members of the group (and any seminar fees
requested from the participants, described in Article III.5) are used
for the maintenance and development of Computree core elements, the
tests as well as the animation of the Computree community, dissemination
and documentation of the platform.

Note that the development of plugins and libraries is not funded on
resources allocated to the platform, excepting plugins and libraries
listed among central elements, whose development has been validated by
the strategic committee. This list is kept up to date by the Computree
Manager.

The use of Computree and the resources made available to enable and
facilitate the development of plugins are completely free. Online
resources are made available to facilitate the independence of Computree
community members. An exchange forum also aims to promote mutual support
in the community.

## 5\. Organization of seminars

“Plugin developers” seminars are proposed according to the needs. They
are free for any plugin released under Open-Source license and remaining
in the fields of application of natural environments or forest. In all
other cases, they are subject to payment on invoice of seminar fees.

Each year, at least one Computree User Seminar is offered to the
community, possibly several depending on demand and needs. These
seminars are intended to introduce users to the use of the Computree
platform. Thematic deepening seminars may also be offered. These
seminars may be subject to payment on invoice of seminar fees incurred
by each trainee. For ease of access, Computree User Seminars can also be
offered in the training catalogue of one or more group member
organizations.

The resources resulting from the organization of these seminars (if any)
feed into the budget allowing the evolution of the platform. In
addition, members of the Computree community are free to organize their
own Computree seminars or training.

 

# IV. EVOLUTION OF THE COMPOSITION OF THE COMPUTREE GROUP

## 1\. Conditions of admission and obligations of new members

When a new member wishes to join the Computree group, a draft agreement
with the Computree Group Leader - specifying, if applicable, past
contributions to the core elements of the platform, and in any case the
contributions planned for the period of the convention - is drafted
before the meeting of the strategic committee. During the meeting, the
strategic committee considers and decides on the integration of the new
member by a two-thirds majority. Once the integration is validated and
the agreement between the new member and the Computree Group Leader
signed by both parties, the new member joins the Computree Group and
sits on the Strategy Committee.

New members undertake to respect the obligations incumbent on them and
the provisions of this charter.

New members commit to allocate significant resources (equivalent value
greater than or equal to 5,000 € / year), over time, to maintain or
develop the core elements of the Computree platform. These resources are
defined in a multi-year agreement with the Computree Group Leader. They
can be of different natures:

  - Financial amount.
  - Staff time made available.
  - Material resources made available.

In all cases, these resources are converted into an equivalent monetary
amount, in order to determine the voting rights of each member of the
group, in proportion to his contribution for the current year.

## 2\. Conditions of exclusion of a member

In the event that one of the members fails to comply with the
obligations set out in this charter, and after a formal notice from the
Computree Group Leader has been ineffective for a period of three
months, the Strategy Committee may decide to exclude him (by unanimity,
the member at stake not participating in this vote), noting that the
contributions paid are not returned to him.

In this case, the Strategic Committee mandates the Computree Group
Leader to terminate the agreement with the member at stake. The
termination date will be set at the end of the financial year, as far as
possible.

## 3\. Renewal Terms for Active Members

At the expiry of the agreement of one of the members of the group, this
member may automatically decide on its renewal (by amendment or new
agreement), unless he has violated of the provisions of this charter.

However, if the contribution is increased by more than 50%, the
strategic committee must agree to this increase (absolute majority vote,
the member at stake not participating in this vote).

The voting rights will be recalculated according to the terms defined in
Article III.2.

 

# V. RESPONSIBILITIES / WARRANTY

**The Computree platform is made available “as is” without warranty of
any kind, either express or implied, including, but not limited to, the
implied warranties of merchantability and fitness for a particular
purpose. The entire risk as of quality and performance of the platform
is supported by the user. If the platform is defective, the users bear
alone the cost of any necessary repairs.**

Computree Group members are not responsible for the use made by users of
the Platform or the results of the resulting analysis.

 

# VII. GENERAL PROVISIONS

Nothing in this Charter shall be construed as constituting among the
members a legal entity of any nature whatsoever or involving any kind of
solidarity between the members.

The members declare that the present Charter cannot in any case be
interpreted or regarded as constituting an act of society, the affectio
societatis is formally excluded.

No member has the power to engage other members, nor to create
obligations for other members, other than the Computree Group Leader,
for the sole purpose of the assignment and to the extent of the rights
which are granted to it/her/him.

 

\[\[En\_wiki\_v5|Back to home\]\]
