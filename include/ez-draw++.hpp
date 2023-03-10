/*
 * ez-draw++.hpp: basic EZ-Draw++ module
 *
 * eric.remy@univ-amu.fr - 30/03/2022 - version 1.2-6
 *
 * This program is free software under the terms of the
 * GNU Lesser General Public License (LGPL) version 2.1.
 *
 * ez-draw++ is based on ez-draw (see below)
*/

/// \file
/// Le fichier d'en-tête C++ nécessaire à l'utilisation des fonctionnalités d'EZ-Draw++.

#ifndef EZDRAWPP_HPP
#define EZDRAWPP_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdarg>
#include <map>

/// Type entier 8 bits non-signé permettant de représenter une valeur entre 0 et 255 inclus.
typedef unsigned char  EZuint8;
/// Classe permettant de représenter une valeur de couleur.
/// \sa EZWindow::setColor()
/// \sa EZWindow::getRGB()
class EZColor {
  unsigned int value;
  static void init();
public:

 /// Construit une couleur à partir d'un niveau de gris.
 /// Cette fonction tient le rôle de constructeur par défaut et crée une couleur noire si on ne précise pas la valeur de gris désirée.
 /// \param grey le niveau de gris désiré entre 0 et 255.
 explicit EZColor(EZuint8 grey=0);
 /// Construit une couleur à partir les valeurs de ses composantes rouge, verte et bleue :
 /// \param red la composante de rouge, entre 0 et 255 ;
 /// \param green la composante de vert, entre 0 et 255 ;
 /// \param blue la composante de bleu, entre 0 et 255.
 inline EZColor(int red,int green,int blue) : EZColor(EZuint8(red),EZuint8(green),EZuint8(blue)) {}
 /// Construit une couleur à partir les valeurs de ses composantes rouge, verte et bleue (exprimées sur 8 bits) :
 /// \param red la composante de rouge, entre 0 et 255 ;
 /// \param green la composante de vert, entre 0 et 255 ;
 /// \param blue la composante de bleu, entre 0 et 255.
 explicit EZColor(EZuint8 red,EZuint8 green,EZuint8 blue);
 /// Construit une couleur à partir les valeurs de ses composantes de teinte, saturation et valeur :
 /// \param hue la composante de teinte, réelle entre 0 et 360 ;
 /// \param saturation la composante de saturation, réelle entre 0 et 1 ;
 /// \param value la composante de valeur, réelle entre 0 et 1.
 /// \sa Wikipédia : <a href="https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur">Teinte, Saturation et Valeur</a>
 explicit EZColor(double hue,double saturation,double value);
 /// Accesseur de consultation pour obtenir la valeur de la composante rouge de la EZColor.
 EZuint8 getRed() const;
 /// Accesseur de consultation pour obtenir la valeur de la composante verte de la EZColor.
 EZuint8 getGreen() const;
 /// Accesseur de consultation pour obtenir la valeur de la composante bleue de la EZColor.
 EZuint8 getBlue() const;
 /// Accesseur de modification qui permet de modifier la valeur de composante rouge de la EZColor.
 void setRed(EZuint8 red);
 /// Accesseur de modification qui permet de modifier la valeur de composante vert de la EZColor.
 void setGreen(EZuint8 green);
 /// Accesseur de modification qui permet de modifier la valeur de composante bleue de la EZColor.
 void setBlue(EZuint8 blue);
 /// Écrit la valeur de couleur avec la notation Web classique, « \#FF0000 » par exemple pour du rouge pur.
 friend std::ostream& operator<<(std::ostream& os,const EZColor& col);
 /// Lit une valeur de couleur à partir de sa représentation Web classique, par exemple « \#0000FF » pour du bleu pur.
 friend std::istream& operator>>(std::istream& is,      EZColor& col);
/// @cond Private_implantation
 friend class EZWindow;
 friend class EZDraw;
/// @endcond

/// Ensemble de constantes permettant de dessiner simplement dans les couleurs les plus fréquentes (sans devoir
/// utiliser EZWindow::getRGB() par exemple).
 static EZColor black,   ///< Noir
                white,   ///< Blanc
                grey,    ///< Gris
                red,     ///< Rouge
                green,   ///< Vert
                blue,    ///< Bleu
                yellow,  ///< Jaune
                cyan,    ///< Cyan
                magenta; ///< Magenta
};


/// \enum EZAlign Le type énuméré EZAlign contient un ensemble de constantes permettant d'exprimer la façon ton un texte doit être aligné lors de son écriture dans une fenêtre par rapport au point de référence.
/// Les constantes sont formées par la juxtaposition des initiales des mots anglais servant à indiquer :
/// - la position verticale (haut, milieu, bas) ;
/// - la position horizontale (cadré à gauche, centré, cadré à droite) ;
/// - éventuellement, si un rectangle blanc doit être tracé sous l'emplacement du texte pour garantir sa lisibilité.
/// Pour utiliser une de ces valeurs, il faut préciser le nom de l'énumération qui la contient. Ainsi, pour indiquer un placement
/// en haut, à gauche et sur fond rempli en blanc par exemple, il faut utiliser la constante « EZAlign::TLF ».
/// \sa void EZWindow::drawText(EZAlign align,int x,int y,const char* str) const
enum class EZAlign : int {
/// @cond Private_implantation
    AA = 183200,
/// @endcond
    TL,     ///< Top Left
    TC,     ///< Top Center
    TR,     ///< Top Right
    ML,     ///< Middle Left
    MC,     ///< Middle Center
    MR,     ///< Middle Right
    BL,     ///< Bottom Left
    BC,     ///< Bottom Center
    BR,     ///< Bottom Right
/// @cond Private_implantation
    BB,
/// @endcond
    TLF,     ///< Top Left with background filled
    TCF,     ///< Top Center with background filled
    TRF,     ///< Top Right with background filled
    MLF,     ///< Middle Left with background filled
    MCF,     ///< Middle Center with background filled
    MRF,     ///< Middle Right with background filled
    BLF,     ///< Bottom Left with background filled
    BCF,     ///< Bottom Center with background filled
    BRF,     ///< Bottom Right with background filled
//  CC // Constante EZ-Draw supprimée parce qu'elle est inutile à EZ-Draw++ et qu'elle fait déconner le mécanisme d'exclusion de doxygen.
};

/// \enum EZKeySym Ce type énuméré rassemble la liste des identifiants permettant d'indiquer une touche de clavier.
/// Pour utiliser une de ces valeurs, il faut préciser le nom de l'énumération qui la contient. Ainsi par exemple, pour la touche « A »,
/// il faut utiliser la constante « EZKeySym::A ».
/// Consulter le tableau ci-dessous pour connaître les symboles existants.
/// Vous pouvez aussi utiliser le programme `demo++05` pour obtenir le nom du symbole en testant en direct l'appui sur une touche.
/// Vous pouvez écrire sur un flux ou à l'écran le caractère correspondant à un EZKeySym grâce à la fonction operator<<(std::ostream& os,const EZKeySym& k).
enum class EZKeySym
{
 BackSpace                    = 0xff08, ///< le retour arrière « <-- »
 Tab                          = 0xff09, ///< la tabulation « -->| »
 Return                       = 0xff0d, ///< la touche Entrée
 Pause                        = 0xff13, ///< la touche pause
 Scroll_Lock                  = 0xff14, ///< la touche Arrêt de défil(ement)
 Escape                       = 0xff1b, ///< la touche Échap(pement)
 Delete                       = 0xffff, ///< Suppr(ession)
 Home                         = 0xff50, ///< Début « |< »
 Left                         = 0xff51, ///< flèche vers la gauche
 Up                           = 0xff52, ///< flèche vers le haut
 Right                        = 0xff53, ///< flèche vers la droite
 Down                         = 0xff54, ///< flèche vers le bas
 Prior                        = 0xff55, ///< touche Page Précédente
 Next                         = 0xff56, ///< touche Page Suivante
 End                          = 0xff57, ///< Fin « >| »
 Insert                       = 0xff63, ///< Inser(tion)
 Menu                         = 0xff67, ///< Menu
 Num_Lock                     = 0xff7f, ///< Ver(rouillage) Num(érique)
 KP_Enter                     = 0xff8d, ///< touche Entrée du pavé numérique
 KP_Home                      = 0xff95, ///< touche Début du pavé numérique
 KP_Left                      = 0xff96, ///< touche Gauche du pavé numérique
 KP_Up                        = 0xff97, ///< touche Haut du pavé numérique
 KP_Right                     = 0xff98, ///< touche Droite du pavé numérique
 KP_Down                      = 0xff99, ///< touche Gauche du pavé numérique
 KP_Prior                     = 0xff9a, ///< touche Page Précédente du pavé numérique
 KP_Next                      = 0xff9b, ///< touche Page Suivante du pavé numérique
 KP_End                       = 0xff9c, ///< touche Fin du pavé numérique
 KP_Begin                     = 0xff9d, ///< touche Début du pavé numérique
 KP_Equal                     = 0xffbd, ///< touche « = » du pavé numérique
 KP_Multiply                  = 0xffaa, ///< touche « * » du pavé numérique
 KP_Add                       = 0xffab, ///< touche « + » du pavé numérique
 KP_Separator                 = 0xffac, ///< touche Separateur du pavé numérique
 KP_Subtract                  = 0xffad, ///< touche « - » du pavé numérique
 KP_Divide                    = 0xffaf, ///< touche « / » du pavé numérique
 KP_0                         = 0xffb0, ///< touche « 0 » du pavé numérique
 KP_1                         = 0xffb1, ///< touche « 1 » du pavé numérique
 KP_2                         = 0xffb2, ///< touche « 2 » du pavé numérique
 KP_3                         = 0xffb3, ///< touche « 3 » du pavé numérique
 KP_4                         = 0xffb4, ///< touche « 4 » du pavé numérique
 KP_5                         = 0xffb5, ///< touche « 5 » du pavé numérique
 KP_6                         = 0xffb6, ///< touche « 6 » du pavé numérique
 KP_7                         = 0xffb7, ///< touche « 7 » du pavé numérique
 KP_8                         = 0xffb8, ///< touche « 8 » du pavé numérique
 KP_9                         = 0xffb9, ///< touche « 9 » du pavé numérique
 F1                           = 0xffbe, ///< touche de fonction « F1 »
 F2                           = 0xffbf, ///< touche de fonction « F2 »
 F3                           = 0xffc0, ///< touche de fonction « F3 »
 F4                           = 0xffc1, ///< touche de fonction « F4 »
 F5                           = 0xffc2, ///< touche de fonction « F5 »
 F6                           = 0xffc3, ///< touche de fonction « F6 »
 F7                           = 0xffc4, ///< touche de fonction « F7 »
 F8                           = 0xffc5, ///< touche de fonction « F8 »
 F9                           = 0xffc6, ///< touche de fonction « F9 »
 F10                          = 0xffc7, ///< touche de fonction « F10 »
 F11                          = 0xffc8, ///< touche de fonction « F11 »
 F12                          = 0xffc9, ///< touche de fonction « F12 »
 Shift_L                      = 0xffe1, ///< Majusculte (gauche)
 Shift_R                      = 0xffe2, ///< Majusculte (droite)
 Control_L                    = 0xffe3, ///< trouche Contrôle (gauche)
 Control_R                    = 0xffe4, ///< trouche Contrôle (droite)
 Caps_Lock                    = 0xffe5, ///< Verouillage Majusculte
 Meta_L                       = 0xffe7, ///< touche Meta (gauche)
 Meta_R                       = 0xffe8, ///< touche Meta (droite)
 Alt_L                        = 0xffe9, ///< touche Alt (gauche)
 Alt_R                        = 0xffea, ///< touche Alt (droite)
 space                        = 0x0020, ///< Espace
 exclam                       = 0x0021, ///< Point d'exclamation
 quotedbl                     = 0x0022, ///< Guillemet anglaise : « " »
 numbersign                   = 0x0023, ///< dièse : « # »
 dollar                       = 0x0024, ///< Symbole du dollar : « $ »
 percent                      = 0x0025, ///< Symbole du pourcent : « % »
 ampersand                    = 0x0026, ///< Esperluette (« Et commercial ») : « & »
 apostrophe                   = 0x0027, ///< Apostrophe
 parenleft                    = 0x0028, ///< Parenthèse ouvrante
 parenright                   = 0x0029, ///< Parenthèse fermante
 asterisk                     = 0x002a, ///< Astérisque
 plus                         = 0x002b, ///< Signe plus
 comma                        = 0x002c, ///< Virgule
 minus                        = 0x002d, ///< Signe moins
 period                       = 0x002e, ///< Point
 slash                        = 0x002f, ///< Barre oblique : « / »
 _0                           = 0x0030, ///< Chiffre 0
 _1                           = 0x0031, ///< Chiffre 1
 _2                           = 0x0032, ///< Chiffre 2
 _3                           = 0x0033, ///< Chiffre 3
 _4                           = 0x0034, ///< Chiffre 4
 _5                           = 0x0035, ///< Chiffre 5
 _6                           = 0x0036, ///< Chiffre 6
 _7                           = 0x0037, ///< Chiffre 7
 _8                           = 0x0038, ///< Chiffre 8
 _9                           = 0x0039, ///< Chiffre 9
 colon                        = 0x003a, ///< Symbole deux points
 semicolon                    = 0x003b, ///< Symbole point virgule
 less                         = 0x003c, ///< Symbole inférieur à
 equal                        = 0x003d, ///< Sumbole égual à
 greater                      = 0x003e, ///< Symbole suppérieur à
 question                     = 0x003f, ///< Point d'interogation
 at                           = 0x0040, ///< Arobase : « @ »
 A                            = 0x0041, ///< A (majuscule)
 B                            = 0x0042, ///< B (majuscule)
 C                            = 0x0043, ///< C (majuscule)
 D                            = 0x0044, ///< D (majuscule)
 E                            = 0x0045, ///< E (majuscule)
 F                            = 0x0046, ///< F (majuscule)
 G                            = 0x0047, ///< G (majuscule)
 H                            = 0x0048, ///< H (majuscule)
 I                            = 0x0049, ///< I (majuscule)
 J                            = 0x004a, ///< J (majuscule)
 K                            = 0x004b, ///< K (majuscule)
 L                            = 0x004c, ///< L (majuscule)
 M                            = 0x004d, ///< M (majuscule)
 N                            = 0x004e, ///< N (majuscule)
 O                            = 0x004f, ///< O (majuscule)
 P                            = 0x0050, ///< P (majuscule)
 Q                            = 0x0051, ///< Q (majuscule)
 R                            = 0x0052, ///< R (majuscule)
 S                            = 0x0053, ///< S (majuscule)
 T                            = 0x0054, ///< T (majuscule)
 U                            = 0x0055, ///< U (majuscule)
 V                            = 0x0056, ///< V (majuscule)
 W                            = 0x0057, ///< W (majuscule)
 X                            = 0x0058, ///< X (majuscule)
 Y                            = 0x0059, ///< Y (majuscule)
 Z                            = 0x005a, ///< Z (majuscule)
 bracketleft                  = 0x005b, ///< Crochet ouvrant : « [ »
 backslash                    = 0x005c, ///< Barre oblique renversée : « \ »
 bracketright                 = 0x005d, ///< Crochet fermant : « ] »
 asciicircum                  = 0x005e, ///< Accent circonflexe (seul) : « ^ »
 underscore                   = 0x005f, ///< Carractère « souligné » : « _ »
 grave                        = 0x0060, ///< Accent grave (seul) : « ` »
 a                            = 0x0061, ///< a (minuscule)
 b                            = 0x0062, ///< b (minuscule)
 c                            = 0x0063, ///< c (minuscule)
 d                            = 0x0064, ///< d (minuscule)
 e                            = 0x0065, ///< e (minuscule)
 f                            = 0x0066, ///< f (minuscule)
 g                            = 0x0067, ///< g (minuscule)
 h                            = 0x0068, ///< h (minuscule)
 i                            = 0x0069, ///< i (minuscule)
 j                            = 0x006a, ///< j (minuscule)
 k                            = 0x006b, ///< k (minuscule)
 l                            = 0x006c, ///< l (minuscule)
 m                            = 0x006d, ///< m (minuscule)
 n                            = 0x006e, ///< n (minuscule)
 o                            = 0x006f, ///< o (minuscule)
 p                            = 0x0070, ///< p (minuscule)
 q                            = 0x0071, ///< q (minuscule)
 r                            = 0x0072, ///< r (minuscule)
 s                            = 0x0073, ///< s (minuscule)
 t                            = 0x0074, ///< t (minuscule)
 u                            = 0x0075, ///< u (minuscule)
 v                            = 0x0076, ///< v (minuscule)
 w                            = 0x0077, ///< w (minuscule)
 x                            = 0x0078, ///< x (minuscule)
 y                            = 0x0079, ///< y (minuscule)
 z                            = 0x007a, ///< z (minuscule)
 braceleft                    = 0x007b, ///< Accolade ouvrante : « { »
 bar                          = 0x007c, ///< Barre verticale : « | »
 braceright                   = 0x007d, ///< Accolade fermante : « } »
 asciitilde                   = 0x007e, ///< Accent tilde (seul)
 nobreakspace                 = 0x00a0, ///< Espace non-sécable
 exclamdown                   = 0x00a1,  /* INVERTED EXCLAMATION MARK */
 cent                         = 0x00a2,  /* CENT SIGN */
 sterling                     = 0x00a3,  /* POUND SIGN */
 currency                     = 0x00a4,  /* CURRENCY SIGN */
 yen                          = 0x00a5,  /* YEN SIGN */
 brokenbar                    = 0x00a6,  /* BROKEN BAR */
 section                      = 0x00a7,  /* SECTION SIGN */
 diaeresis                    = 0x00a8,  /* DIAERESIS */
 copyright                    = 0x00a9,  /* COPYRIGHT SIGN */
 ordfeminine                  = 0x00aa,  /* FEMININE ORDINAL INDICATOR */
 guillemotleft                = 0x00ab,  /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
 notsign                      = 0x00ac,  /* NOT SIGN */
 hyphen                       = 0x00ad,  /* SOFT HYPHEN */
 registered                   = 0x00ae,  /* REGISTERED SIGN */
 macron                       = 0x00af,  /* MACRON */
 degree                       = 0x00b0,  /* DEGREE SIGN */
 plusminus                    = 0x00b1,  /* PLUS-MINUS SIGN */
 twosuperior                  = 0x00b2,  ///< exposant deux
 threesuperior                = 0x00b3,  /* SUPERSCRIPT THREE */
 acute                        = 0x00b4,  /* ACUTE ACCENT */
 mu                           = 0x00b5,  /* MICRO SIGN */
 paragraph                    = 0x00b6,  /* PILCROW SIGN */
 periodcentered               = 0x00b7,  /* MIDDLE DOT */
 cedilla                      = 0x00b8,  /* CEDILLA */
 onesuperior                  = 0x00b9,  /* SUPERSCRIPT ONE */
 masculine                    = 0x00ba,  /* MASCULINE ORDINAL INDICATOR */
 guillemotright               = 0x00bb,  /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
 onequarter                   = 0x00bc,  /* VULGAR FRACTION ONE QUARTER */
 onehalf                      = 0x00bd,  /* VULGAR FRACTION ONE HALF */
 threequarters                = 0x00be,  /* VULGAR FRACTION THREE QUARTERS */
 questiondown                 = 0x00bf,  /* INVERTED QUESTION MARK */
 Agrave                       = 0x00c0,  /* CAPITAL LETTER A WITH GRAVE */
 Aacute                       = 0x00c1,  /* CAPITAL LETTER A WITH ACUTE */
 Acircumflex                  = 0x00c2,  /* CAPITAL LETTER A WITH CIRCUMFLEX */
 Atilde                       = 0x00c3,  /* CAPITAL LETTER A WITH TILDE */
 Adiaeresis                   = 0x00c4,  /* CAPITAL LETTER A WITH DIAERESIS */
 Aring                        = 0x00c5,  /* CAPITAL LETTER A WITH RING ABOVE */
 AE                           = 0x00c6,  /* CAPITAL LETTER AE */
 Ccedilla                     = 0x00c7,  /* CAPITAL LETTER C WITH CEDILLA */
 Egrave                       = 0x00c8,  /* CAPITAL LETTER E WITH GRAVE */
 Eacute                       = 0x00c9,  /* CAPITAL LETTER E WITH ACUTE */
 Ecircumflex                  = 0x00ca,  /* CAPITAL LETTER E WITH CIRCUMFLEX */
 Ediaeresis                   = 0x00cb,  /* CAPITAL LETTER E WITH DIAERESIS */
 Igrave                       = 0x00cc,  /* CAPITAL LETTER I WITH GRAVE */
 Iacute                       = 0x00cd,  /* CAPITAL LETTER I WITH ACUTE */
 Icircumflex                  = 0x00ce,  /* CAPITAL LETTER I WITH CIRCUMFLEX */
 Idiaeresis                   = 0x00cf,  /* CAPITAL LETTER I WITH DIAERESIS */
 ETH                          = 0x00d0,  /* CAPITAL LETTER ETH */
 Ntilde                       = 0x00d1,  /* CAPITAL LETTER N WITH TILDE */
 Ograve                       = 0x00d2,  /* CAPITAL LETTER O WITH GRAVE */
 Oacute                       = 0x00d3,  /* CAPITAL LETTER O WITH ACUTE */
 Ocircumflex                  = 0x00d4,  /* CAPITAL LETTER O WITH CIRCUMFLEX */
 Otilde                       = 0x00d5,  /* CAPITAL LETTER O WITH TILDE */
 Odiaeresis                   = 0x00d6,  /* CAPITAL LETTER O WITH DIAERESIS */
 multiply                     = 0x00d7,  /* MULTIPLICATION SIGN */
 Oslash                       = 0x00d8,  /* CAPITAL LETTER O WITH STROKE */
 Ugrave                       = 0x00d9,  /* CAPITAL LETTER U WITH GRAVE */
 Uacute                       = 0x00da,  /* CAPITAL LETTER U WITH ACUTE */
 Ucircumflex                  = 0x00db,  /* CAPITAL LETTER U WITH CIRCUMFLEX */
 Udiaeresis                   = 0x00dc,  /* CAPITAL LETTER U WITH DIAERESIS */
 Yacute                       = 0x00dd,  /* CAPITAL LETTER Y WITH ACUTE */
 THORN                        = 0x00de,  /* CAPITAL LETTER THORN */
 ssharp                       = 0x00df,  /* SMALL LETTER SHARP S */
 agrave                       = 0x00e0,  /* SMALL LETTER A WITH GRAVE */
 aacute                       = 0x00e1,  /* SMALL LETTER A WITH ACUTE */
 acircumflex                  = 0x00e2,  /* SMALL LETTER A WITH CIRCUMFLEX */
 atilde                       = 0x00e3,  /* SMALL LETTER A WITH TILDE */
 adiaeresis                   = 0x00e4,  /* SMALL LETTER A WITH DIAERESIS */
 aring                        = 0x00e5,  /* SMALL LETTER A WITH RING ABOVE */
 ae                           = 0x00e6,  /* SMALL LETTER AE */
 ccedilla                     = 0x00e7,  /* SMALL LETTER C WITH CEDILLA */
 egrave                       = 0x00e8,  /* SMALL LETTER E WITH GRAVE */
 eacute                       = 0x00e9,  /* SMALL LETTER E WITH ACUTE */
 ecircumflex                  = 0x00ea,  /* SMALL LETTER E WITH CIRCUMFLEX */
 ediaeresis                   = 0x00eb,  /* SMALL LETTER E WITH DIAERESIS */
 igrave                       = 0x00ec,  /* SMALL LETTER I WITH GRAVE */
 iacute                       = 0x00ed,  /* SMALL LETTER I WITH ACUTE */
 icircumflex                  = 0x00ee,  /* SMALL LETTER I WITH CIRCUMFLEX */
 idiaeresis                   = 0x00ef,  /* SMALL LETTER I WITH DIAERESIS */
 eth                          = 0x00f0,  /* SMALL LETTER ETH */
 ntilde                       = 0x00f1,  /* SMALL LETTER N WITH TILDE */
 ograve                       = 0x00f2,  /* SMALL LETTER O WITH GRAVE */
 oacute                       = 0x00f3,  /* SMALL LETTER O WITH ACUTE */
 ocircumflex                  = 0x00f4,  /* SMALL LETTER O WITH CIRCUMFLEX */
 otilde                       = 0x00f5,  /* SMALL LETTER O WITH TILDE */
 odiaeresis                   = 0x00f6,  /* SMALL LETTER O WITH DIAERESIS */
 division                     = 0x00f7,  /* DIVISION SIGN */
 oslash                       = 0x00f8,  /* SMALL LETTER O WITH STROKE */
 ugrave                       = 0x00f9,  /* SMALL LETTER U WITH GRAVE */
 uacute                       = 0x00fa,  /* SMALL LETTER U WITH ACUTE */
 ucircumflex                  = 0x00fb,  /* SMALL LETTER U WITH CIRCUMFLEX */
 udiaeresis                   = 0x00fc,  /* SMALL LETTER U WITH DIAERESIS */
 yacute                       = 0x00fd,  /* SMALL LETTER Y WITH ACUTE */
 thorn                        = 0x00fe,  /* SMALL LETTER THORN */
 ydiaeresis                   = 0x00ff,  /* SMALL LETTER Y WITH DIAERESIS */
 EuroSign                     = 0x20ac   /* EURO SIGN */
};

/// Opérateur d'injection d'un EZKeySym sur un flux.
/// Le résultat obtenu est d'écrire le caractère correspondant (et pas le nom de la constante associée, c'est-à-dire « a » et pas « EZKeySym::a ») sur le flux.
std::ostream& operator<<(std::ostream& os,const EZKeySym& k);

/// @cond Private_implantation
struct sEz_event;
/// @endcond

/// La classe EZEvent représente un événement à traiter de l'interface graphique : clic de la souris, touche du clavier pressée ou relachée, etc.
class EZEvent {
 /// @cond Private_implantation
 const struct sEz_event* ez_event;
 inline void setEvent(const struct sEz_event *e) { ez_event = e; }
 EZEvent(const struct sEz_event *);
 /// @endcond
public:
 /// Pour un événement concernant la souris, renvoie la coordonnée x de la position de la souris au moment où l'événement s'est déclenché.
 int mouseX() const;
 /// Pour un événement concernant la souris, renvoie la coordonnée y de la position de la souris au moment où l'événement s'est déclenché.
 int mouseY() const;
 /// Pour un événement concernant la souris, renvoie le numéro du bouton de la souris qui a déclenché l'événement en étant appuyé ou relaché.
 int mouseButton() const;
 /// Renvoie l'identificant de la touche du clavier qui a été appuyée/relachée sous forme d'un EZKeySym.
 EZKeySym     keySym() const;
 /// Renvoie une chaîne de caractère permettant d'afficher le nom du EZKeySym.
 std::string keyName() const;
 /// Renvoie le nombre de caractères qui sont stockés dans la chaîne consultable par const char * EZEvent::keyString() const.
 std::size_t       keyCount() const;
 /// Renvoie le(s) caractère(s) qui sont associés à la touche du clavier qui a été appuyée/relachée.
 /// Le nombre de caractères est consultable avec  size_t EZEvent::keyCount() const.
 const char * keyString() const;
 friend class EZWindow;
};

/// La classe EZWindow correspond à une fenêtre sous Windows ou sous UNIX.
/// Cette classe se charge de toute les opérations nécessaires pour la création et le fonctionnement normal d'une fenêtre sur votre système d'exploitation Windows ou Linux/X-Window.
class EZWindow {
/// @cond Private_implantation
 EZEvent current_event;
 bool _isVisible;
                         EZWindow     (const EZWindow&) = delete;
        const EZWindow&  operator=    (const EZWindow&) = delete;
 static void dispatch(struct sEz_event *e);
 /// @endcond
public:
/// Constructeur de EZWindow à partir de ses dimensions et d'un titre.
/// \param width entier qui indique la largeur (par défaut, 320 pixels).
/// \param height entier qui indique la hauteur (par défaut, 200 pixels).
/// \param title une chaîne C++ qui indique le titre voulu (par défaut, une chaîne vide).
                         EZWindow     (int width=320, int height=200, const char *title="");

/// Destructeur virtuel de la classe.
 virtual                 ~EZWindow    ();

/// Règle si on veut activer le « double buffer » pour éviter le scintillement de l'affichage.
/// \note La technique de « double buffer » consiste à ne pas dessiner directement à l'écran mais dans une partie non affichée de la mémoire vidéo, puis une fois que toutes les opérations de dessin sont finies, à recopier de manière rapide la zone à l'écran. On évite ainsi de voir clignoter l'intérieur de la fenêtre pendant que le programme la trace.
/// Par défaut, le « double buffer » n'est pas actif pour une nouvelle fenêtre.
/// \param state ce booléen indique s'il faut activer (true) ou désactiver (false) la technique de « double buffer » pour cette fenêtre.
        void             setDoubleBuffer (bool state=true);


/// Règle la largeur de la fenêtre.
/// \param width entier positif qui sera utilisé comme nouvelle largeur de la fenêtre.
/// \note Attention la modification de la largeur ainsi produite n'est qu'une demande qui s'ajoute à la file d'attente, et qui ne sera pas effective tant que la boucle de traitement d'événement ne l'aura pas prise en compte (cf. « EZDraw::mainLoop() »). De ce fait, si vous souhaitez modifier la largeur ET la hauteur simultanément, il est nécessaire d'utiliser « EZWindow::setWidthHeight(int w,int h) ».
        void             setWidth     (int width);

/// Règle la hauteur de la fenêtre.
/// \param height entier positif qui sera utilisé comme nouvelle hauteur de la fenêtre.
/// \note Attention : la modification de la largeur ainsi produite n'est qu'une demande qui s'ajoute à la file d'attente, et qui ne sera pas effective tant que la boucle de traitement d'événement ne l'aura pas prise en compte (cf. « EZDraw::mainLoop() »). De ce fait, si vous souhaitez modifier la largeur ET la hauteur simultanément, il est nécessaire d'utiliser « EZWindow::setWidthHeight(int w,int h) ».
        void             setHeight    (int height);

/// Règle simultanément la largeur et la hauteur de la fenêtre.
/// \param width entier positif qui sera utilisé comme nouvelle largeur de la fenêtre.
/// \param height entier positif qui sera utilisé comme nouvelle hauteur de la fenêtre.
/// \note Attention : la modification de la largeur ainsi produite n'est qu'une demande qui s'ajoute à la file d'attente, et qui ne sera pas effective tant que la boucle de traitement d'événement ne l'aura pas prise en compte (cf. « EZDraw::mainLoop() »).
        void             setWidthHeight(int width, int height);

/// Renvoie la largeur actuelle de la fenêtre.
        int             getWidth     () const;

/// Renvoie la hauteur actuelle de la fenêtre.
        int             getHeight    () const;

/// Indique si la fenêtre est visible à l'écran.
/// \return Si la valeur renvoyée est true alors la fenêtre est visible à l'écran (mais elle peut être partiellement sortie au delà du bord de l'affichage). Si la valeur est false, c'est que la fenêtre existe mais n'est pas dessinée à l'écran.
/// \sa setVisible(bool visible)
        bool             isVisible    () const { return _isVisible; }

/// Rend visible ou cache la fenêtre.
/// \param visible booléen indiquant si vous voulez rendre une fenêtre visible (true) ou si au contraire, vous voulez la cacher (sans la détruire) (false).
/// \sa isVisible    () const
        void             setVisible   (bool visible=true);

/// Efface la totalité de l'intérieur de la fenêtre avec un fond uni de couleur blanche.
/// Cette fonction réinitialise également les paramètres de dessin.
        void             reset        () const;

/// Efface la totalité de l'intérieur de la fenêtre avec un fond uni de la couleur indiquée.
        void             clear        (EZColor color) const;

/// Règle la couleur des prochaines opérations de dessin.
/// À moins de n'utiliser que les couleurs prédéfinies (ez_black, ez_white, ez_grey, ez_red, ez_green, ez_blue, ez_yellow, ez_cyan ou ez_magenta), obtenir une valeur de couleur se fait à partir des fonctions getRGB() ou getGrey().
/// \param color entier long correspondant au choix de couleur désiré.
        void             setColor     (EZColor color);

/// Règle l'épaisseur de trait des prochains dessins.
/// \param thick : épaisseur entière en nombre de pixels.
        void             setThick     (int thick=1);

/// Charge une nouvelle police de caractère à partir d'un fichier installé sur le système d'exploitation.
/// Plusieurs polices sont chargées d'origine par EZDraw++ dans les premiers numéros :
/// num | name
/// ----|-----
///  0  | "6x13"
///  1  | "8x16"
///  2  | "10x20"
///  3  | "12x24"
/// \param num Un numéro qui servira à désigner cette police (cf. setFont()) et qui doit être libre, c'est-à-dire supérieur ou égal à 5.
/// \param name Le nom d'une police du système d'exploitation.
/// \warning Sous X11, le nom peut avoir une forme quelconque mais doit correspondre à une fonte existante. Sous Windows, le nom doit être sous la forme largeurxhauteur (une fonte approchante de taille fixe est obtenue).
        void              loadFont     (int num,const char *name);

/// Permet de sélectionner une police déjà chargée en mémoire pour que les écritures ultérieures l'utilisent.
/// \param num Le numéro indiquant quelle police utiliser doit avoir été obtenu par loadFont().
        void             setFont      (int num);

/// Dessine un point à la coordonnée indiquée.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x,y coordonnées du point à tracer.
        void             drawPoint    (int x, int y) const;

/// Dessine une ligne entre les coordonnées indiquées.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du point initial ;
/// \param x2,y2 coordonnées du point final du tracé.
        void             drawLine     (int x1, int y1, int x2, int y2) const;

/// Dessine un rectangle vide entre les coordonnées indiquées.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du point initial ;
/// \param x2,y2 coordonnées du point final du tracé.
        void             drawRectangle(int x1, int y1, int x2, int y2) const;

/// Dessine un rectangle plein entre les coordonnées indiquées.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du point initial ;
/// \param x2,y2 coordonnées du point final du tracé.
        void             fillRectangle(int x1, int y1, int x2, int y2) const;

/// Dessine une ellipse vide inscrite au sein des coordonnées indiquées.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du point initial ;
/// \param x2,y2 coordonnées du point final du tracé.
        void             drawCircle   (int x1, int y1, int x2, int y2) const;

/// Dessine une ellipse pleine inscrite au sein des coordonnées indiquées.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du point initial ;
/// \param x2,y2 coordonnées du point final du tracé.
        void             fillCircle   (int x1, int y1, int x2, int y2) const;

/// Dessine un triangle vide à partir des coordonnées de ses trois sommets.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du premier point ;
/// \param x2,y2 coordonnées du deuxième point ;
/// \param x3,y3 coordonnées du troisième point.
        void             drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3) const;

/// Dessine un triangle plein à partir des coordonnées de ses trois sommets.
/// Le dessin utilise la couleur et l'épaisseur courante.
/// \param x1,y1 coordonnées du premier point ;
/// \param x2,y2 coordonnées du deuxième point ;
/// \param x3,y3 coordonnées du troisième point.
        void             fillTriangle (int x1, int y1, int x2, int y2, int x3, int y3) const;

/// Dessine le texte dans la fenêtre à la position indiquée.
/// \param align L'alignement désiré pour le positionnement du texte (consulter l'aide sur le type EZAlign pour en savoir plus)
/// \param x,y Les coordonnées de placement du texte dans la fenêtre.
/// \param str La chaîne C++ (std::string) à imprimer.
        void             drawText     (EZAlign align,int x,int y,const std::string &str) const;

/// Dessine le texte dans la fenêtre à la position indiquée.
/// \param align L'alignement désiré pour le positionnement du texte (consulter l'aide sur le type EZAlign pour en savoir plus)
/// \param x,y Les coordonnées de placement du texte dans la fenêtre.
/// \param str La chaîne C (const char*) à imprimer.
/// \sa void EZWindow::drawText(EZAlign align,int x,int y,const std::string &str) const;
        void             drawText     (EZAlign align,int x,int y,const char *str) const;

/// Demande à la boucle de gestion d'événements de prévoir un rafraîchissement du dessin de la fenêtre.
/// \sa EZDraw::mainLoop()
        void             sendExpose   () const;

/// Démarre un compte à rebours de la durée indiquée.
/// Une fois le délais du compte à rebours écoulé, la méthode timerNotify() sera appelée sur votre fenêtre. Si vous voulez que le décompte reprenne à nouveau, il suffit de rappeler startTimer() à nouveau en précisant le nouveau délai souhaité.
        void             startTimer   (unsigned int delay) const;

/// Stoppe le compte à rebours associé à cette fenêtre.
/// Utiliser startTimer() pour le relancer avec un nouveau délai.
        void             stopTimer    () const;

 // Gestion des Événements
 /// Accesseur pour l'événement en cours d'analyse sur la file des événements.
 inline  const EZEvent& currentEvent () const { return current_event; }

 /// Cette fonction virtuelle est déclenchée lorsque votre fenêtre doit être redessinée (ne serait-ce que partiellement).
 /// C'est dans cette fonction que vous pouvez réaliser des opérations graphiques. Normalement, il n'est pas souhaitable d'en faire à un autre moment du programme.
 virtual void            expose       ();

 /// Cette fonction virtuelle est déclenchée lorsque l'utilisateur a réclamé la fermeture de cette fenêtre en cliquant sur le bouton prévu à cet effet, en général une croix, dans la bordure de fenêtre qui dépend du système d'exploitation.
 virtual void            close        ();

 /// Cette fonction virtuelle est déclenchée à chaque fois qu'un des boutons de la souris est appuyé.
 /// Dans la plupart des interfaces graphiques actuelles, les actions ne sont pas déclenchées par l'appui mais par le relâchement qui suit. Le glissé-déposé (drag and drop) est un contre exemple : il y a une action de sélection d'un objet à l'enfoncement, une action de suivi à chaque mouvement de la souris et enfin une action d'envoi à un autre objet au moment où la souris est relâchée.
 /// \param mouse_x,mouse_y Les coordonnées de la souris au moment du clic.
 /// \param button Le numéro du bouton qui a été appuyé.
 /// \sa void buttonRelease(int mouse_x,int mouse_y,int button)
 /// \sa void motionNotify (int mouse_x,int mouse_y,int button)
 virtual void            buttonPress  (int mouse_x,int mouse_y,int button);

 /// Cette fonction virtuelle est déclenchée à chaque relâchement d'un bouton de la souris.
 /// \sa void            buttonPress  (int mouse_x,int mouse_y,int button)
 virtual void            buttonRelease(int mouse_x,int mouse_y,int button);

 /// Cette fonction virtuelle est appelée à chaque mouvement de la souris dans la fenêtre.
 /// \warning Cet événement est souvent déclenché de manière très intensive (plusieurs dizaines de fois par seconde) donc il est crucial de limiter à tout prix le temps de traitement qu'elle peut nécessiter.
 /// \sa void            buttonPress  (int mouse_x,int mouse_y,int button)
 virtual void            motionNotify (int mouse_x,int mouse_y,int button);

 /// Cette fonction virtuelle est appelée à chaque fois qu'une touche du clavier est enfoncée et que la fenêtre est sélectionnée (active).
 /// Consulter l'aide de EZKeySym pour plus de détails.
 virtual void            keyPress     (EZKeySym keysym);

 /// Cette fonction virtuelle est appelée à chaque fois qu'une touche du clavier est relâchée et que la fenêtre est sélectionnée (active).
 /// Consulter l'aide de EZKeySym pour plus de détails.
 virtual void            keyRelease   (EZKeySym keysym);

 /// Cette fonction virtuelle est appelée à chaque fois que la taille de la fenêtre est modifiée (à la demande de l'utilisateur par les bordures de fenêtre, ou encore une fois que la demande du programme lui-même avec setWidthHeight() sera effectuée).
 /// \param width,height Les nouvelles dimensions de la fenêtre.
 virtual void            configureNotify(int width,int height);

 /// Cette fonction virtuelle est appelée lorsque le compte à rebours de la fenêtre arrive à zéro.
 /// \sa void startTimer   (unsigned int delay) const
 virtual void            timerNotify  ();

/// @cond Private_implantation
 friend class EZImage;
 friend class EZPixmap;
/// @endcond
};

/// La classe EZDrawError décrit une exception liée aux fonctions de EZDraw++ ou de sa sous-couche interne en C EZ-Draw.
class EZDrawError : public std::runtime_error
{
public:
  /// Constructeur de la classe d'erreur de EZDraw.
  /// \param errmsg le message d'erreur transmis pour décrire l'erreur.
  EZDrawError(const char *errmsg);
};

/// Une instance de la classe EZDraw est nécessaire au bon fonctionnement de votre application.
/// Il est nécessaire que vous en définissiez une et une seule, de préférence en variable locale au « main() », ou en variable globale. Il vous est également possible, voire souhaitable, de dériver cette classe en une classe « MonApplication » dans laquelle vous placeriez d'autres données qui sont communes à toutes les fenêtres de votre application. Ensuite, lors de la création de vos fenêtres, il vous suffit de transmettre une référence à cette instance et de la mémoriser lors de la construction pour que chaque fenêtre puisse accéder à votre instance centrale dérivée de EZDraw.
class EZDraw {
/// @cond Private_implantation
 friend class EZWindow;
 /// Gestionnaire interne pour le déclenchement des exceptions par une erreur C de EZ-Draw.
 static int error_handler(const char *fmt, std::va_list ap);
/// @endcond
public:
 /// Constructeur par défaut.
 /// Le procédé habituel lors de la conception d'une application graphique consiste à dériver par héritage cette classe pour y ajouter les données qui serviront pendant _toute_ la vie de l'application, par exemple la liste de ses fenêtres permanentes.
 /// \sa demo++5.cpp, demo++6.hpp, demo++6.cpp, tracer.cpp
 /// \warning Une instance (et une seule) de la classe EZDraw est nécessaire au bon fonctionnement de votre application.
                      EZDraw();
 /// Destructeur virtuel de la classe EZDraw.
 /// Il est chargé de détruire toutes les données qui sont nécessaires au fonctionnement interne de EZ-Draw++. Vous n'avez normalement pas à vous en soucier tant que vous faites en sorte que l'instance (éventuellement dérivée) de la classe EZDraw que vous crééez pour votre programme est bien détruite à la fin.
 virtual              ~EZDraw();

 /// Cette méthode de classe permet de quitter le programme.
 /// Puisque c'est une méthode de classe, il est possible de l'appeler sur sa classe et pas sur une instance. Il est donc possible de demander la fin du programme en effectuant simplement « EZDraw::quit(); ». Au retour de l'appel à cette fonction, la gestion d'événements finira, en traitant sa file l'attente, par recevoir cet ordre de quitter. À ce moment, la boucle d'événement se termine et la fonction « EZDraw::mainLoop() » se termine et la suite du « main() » peut éventuellement se poursuivre, en particulier pour réaliser la destruction des variables de ce bloc.
        static void   quit();

 /// Indique l'effet désiré d'un click sur le bouton de fermeture d'une des fenêtres de l'application : fermer ou non l'application.
 /// \param value Si value est true, le click sur la croix provoque automatiquement la terminaison de la boucle d'événements (et donc probablement la fin du programme). Si value est false, la fenêtre recevra un événement correspondant à la demande de fermeture de la fenêtre, et c'est au développeur d'indiquer explicitement ce qui doit être fait dans ce cas.
               void   setAutoQuit(bool value);

 /// Boucle d'attente et de traitement des événements.
 /// Tant que cette fonction n'est pas en train de s'éxecuter, aucune fenêtre ne pourra être vue par l'utilisateur, ni aucune action se faire. La sortie de la boucle de gestion des événements qui se trouve à l'intérieur de cette fonction ne devrait avoir que deux origines : soit un appel à EZDraw::quit(), soit un click sur la croix de fermeture d'une fenêtre à la condition que EZDraw::setAutoQuit() ait été appelé avec la valeur true.
               void   mainLoop();

 /// Tire un nombre entier au hasard.
 /// \param n La valeur sera tirée au hasard dans l'intervale compris entre 0 et (n-1) inclus.
 /// \return Le nombre aléatoire tiré.
        static int    random(int n);

 /// La différence entre deux valeurs renvoyées par cette fonction vous permet d'estimer la durée (en secondes) d'un calcul.
 /// \return La fonction renvoie dans un réel double précision le temps écoulé (en secondes) depuis le 1er janvier 1970 à 00:00:00 +0000 (UTC), date dénommée « Epoch » dans le monde de l'informatique et qui sert habituellement d'origine à la mesure informatique du temps dans la plupart des systèmes d'exploitation.
        static double getTime();
};

/// La classe EZPixel représente un pixel dans une EZImage.
/// Un pixel contient trois composantes de couleurs (rouge, vert et bleu) et une valeur de transparence (alpha). Pour que la valeur de transparence soit utilisée, il faut que le booléen has_alpha soit activé dans l'EZImage.
/// \warning Cette classe ne peut pas être utilisée sans la classe EZImage. En fait, elle ne sert que comme valeur de retour de EZImage::getPixel(int x,int y).
class EZPixel {
  EZuint8 *p;
  inline EZPixel(EZuint8 *_p) : p(_p) {}
 public:
  /// Accesseur pour obtenir la valeur de la couleur du pixel.
  inline EZColor getColor() const { return EZColor(p[0],p[1],p[2]); }
  /// Accesseur pour obtenir la valeur de la composante rouge du pixel.
  inline EZuint8 getRed()   const { return p[0]; }
  /// Accesseur pour obtenir la valeur de la composante verte du pixel.
  inline EZuint8 getGreen() const { return p[1]; }
  /// Accesseur pour obtenir la valeur de la composante bleue du pixel.
  inline EZuint8 getBlue()  const { return p[2]; }
  /// Accesseur pour obtenir la valeur de la composante alpha du pixel.
  inline EZuint8 getAlpha() const { return p[3]; }
  /// Accesseur pour modifier la valeur de la couleur du pixel.
  inline void setColor(EZColor col) { p[0] = col.getRed(); p[1] = col.getGreen(); p[2] = col.getBlue(); }
  /// Accesseur pour modifier la valeur de la composante rouge du pixel.
  inline void setRed(EZuint8 r)   { p[0] = r; }
  /// Accesseur pour modifier la valeur de la composante verte du pixel.
  inline void setGreen(EZuint8 g) { p[1] = g; }
  /// Accesseur pour modifier la valeur de la composante bleue du pixel.
  inline void setBlue(EZuint8 b)  { p[2] = b; }
  /// Accesseur pour modifier la valeur de la composante alpha du pixel.
  inline void setAlpha(EZuint8 a) { p[3] = a; }
 friend class EZImage;
};

struct sEz_image;

/// La classe EZImage permet de conserver une image en mémoire puis de la tracer à l'écran.
/// Attention, il n'est pas permis de dériver cette classe par héritage.
class EZImage final {
 struct sEz_image *image;
 EZImage(struct sEz_image*);
 EZImage() = delete;
public:
 /// Constructeur d'une nouvelle image à partir de ses dimensions.
 /// \param width,height la largeur et la hauteur de l'image voulue.
 EZImage(int width, int height);
 /// Constructeur de copie.
 /// \param orig l'image (source) à copier
 EZImage(const EZImage& orig);
 /// Constructeur à partir d'un fichier d'image qui sera chargé en mémoire à partir du disque dur.
 /// Les formats supportés sont : PNG, GIF, BMP, JPEG. Les formats PNG et BMP supportent des degrés variables de transparence. Le format GIF ne permet que de la transparence en tout ou rien. Le format JPEG ne permet pas de représenter la transparence.
 /// \param filename le nom de fichier complet (ou relatif) permettant au programme de trouver le fichier.
 EZImage(const char *filename);
 /// Destructeur de la classe EZImage.
 ~EZImage();
 /// Accesseur pour la largeur actuelle de l'image.
 int getWidth()  const;
 /// Accesseur pour la hauteur actuelle de l'image.
 int getHeight() const;
 /// Permet d'activer ou désactiver la transparence de l'image courante.
 /// \param has_alpha La valeur true active la transparence ; la valeur false la désactive.
 void setAlpha(bool has_alpha);
 /// Cet accesseur permet de savoir si l'image contient de la transparence (true) ou pas (false).
 bool getAlpha() const;
 /// Réglage de la transparence, qu'on appelle alpha channel de l'image.
 /// \param opacity valeur entre 0 (complètement transparent) et 255 (complètement opaque).
 /// Le détail du principe de calcul de transparence est détaillé dans [https://fr.wikipedia.org/wiki/Alpha_blending](cette page Wikipédia).
 void setOpacity(int opacity);
 /// Accesseur pour la valeur de transparence.
 /// \return La valeur retournée est entre 0 (complètement transparent) et 255 (complètement opaque).
 /// Le détail du principe de calcul de transparence est détaillé dans [https://fr.wikipedia.org/wiki/Alpha_blending](cette page Wikipédia).
 int  getOpacity() const;
 /// Accesseur pour obtenir un pixel de coordonnées x et y.
 /// \param x,y Les coordonnées du pixel voulu dans l'image.
 /// \return Une instance de la classe EZPixel qui permet de consulter ou modifier les valeurs de composantes de couleur du pixel.
 EZPixel getPixel(int x,int y);
 /// Affiche l'image dans la fenêtre.
 /// Si has_alpha est vrai, applique la transparence, c’est-à-dire n’affiche que les pixels opaques.
 /// \param win la fenêtre où aura lieu le tracé.
 /// \param x,y les coordonnées du coin supérieur gauche de l’image dans la fenêtre.
 void paint(EZWindow& win,int x,int y) const;
 /// Trace une partie rectangulaire de l'image courante dans la fenêtre.
 /// Si has_alpha est vrai, applique la transparence, c’est-à-dire n’affiche que les pixels opaques.
 /// \param win la fenêtre où aura lieu le tracé.
 /// \param x,y les coordonnées du coin supérieur gauche de l’image dans la fenêtre.
 /// \param src_x,src_y le point du coin supérieur-gauche de la sous-image à extraire.
 /// \param width,height les dimensions largeur et hauteur de la sous-image à extraire.
 void paintSubimage(EZWindow& win,int x, int y,int src_x, int src_y, int width, int height) const;
 /// Affiche une région rectangulaire d’une image dans le terminal.
 void print(int src_x, int src_y, int w, int h) const;
 /// Remplit l'image courante avec la couleur et le degré de transparence indiqué.
 /// \param red,green,blue les trois composantes (rouge, verte et bleue), chacune entre 0 et 255, de la couleur à tracer.
 /// \param alpha le degré de transparence, entre 0 (complètement transparent) et 255 (complètement opaque).
 void fillRGBA(EZuint8 red, EZuint8 green, EZuint8 blue, EZuint8 alpha);
 /// Incruste une partie de l'image src dans l'image courante.
 /// La région de l’image source src est délimitée par les coordonnées src_x, src_y (coin supérieur gauche) et src_x+w-1, src_y+h-1 (coin inférieur droit). Cette région est incrustée dans l’image destination dst aux coordonnées dst_x, dst_y (coin supérieur gauche) et dst_x+w-1, dst_y+h-1 (coin inférieur droit).
 /// Si les coordonnées dépassent les images src ou l'image courante, seule la région commune est incrustée.
 /// Si l’image source n’a pas de canal alpha alors les valeurs de la région de src écrasent celle de dst. Dans le cas contraire, les régions sont mélangées par transparence (alpha blending) avec les formules de [https://fr.wikipedia.org/wiki/Alpha_blending](Porter et Duff).
 /// \param src l'image source de la recopie
 /// \param dst_x,dst_y les coordonnées où le coin supérieur-gauche de l'image src doit arriver dans l'image courante.
 void blend(const EZImage& src, int dst_x, int dst_y);
 /// Idem à la fonction void EZImage::blend(const EZImage& src, int dst_x, int dst_y) mais où il est possible de n'incruster qu'une sous-partie rectangulaire de l'image src.
 /// \param src l'image source de la recopie.
 /// \param dst_x,dst_y les coordonnées où le coin supérieur-gauche de l'image src doit arriver dans l'image courante.
 /// \param src_x,src_y les coordonnées dans l'image src du coin supérieur-gauche de la sous-partie à recopier.
 /// \param width,height les dimensions largeur et hauteur de la sous-partie à recopier.
 void blendSubimage(const EZImage& src, int dst_x, int dst_y,int src_x, int src_y, int width, int height);
 /// Extrait une partie rectangulaire d'une image existante.
 /// \param src_x,src_y la coordonnée du coin supérieur-gauche du rectangle à copier dans l'image d'origine.
 /// \param width,height la largeur et la hauteur de la zone à copier.
 /// \return Renvoie une nouvelle instance de EZImage allouée dynamiquement (et qui devra être détruite grâce à l'opérateur delete) ou nullptr en cas d’erreur.
 EZImage *extract(int src_x, int src_y, int width, int height) const;
 /// Construit une nouvelle image par une symétrie par rapport à l'axe vertical de l'image courante.
 /// \return Renvoie une nouvelle instance de EZImage allouée dynamiquement et qui devra être détruite grâce à l'opérateur delete ou nullptr en cas d’erreur.
 EZImage *verticalSymmetry() const;
 /// Construit une nouvelle image par une symétrie par rapport à l'axe horizontal de l'image courante.
 /// \return Renvoie une nouvelle instance de EZImage allouée dynamiquement et qui devra être détruite grâce à l'opérateur delete ou nullptr en cas d’erreur.
 EZImage *horizontalSymmetry() const;
 /// Construit une nouvelle image par une mise à l'échelle (zoom).
 /// \param factor Le facteur de zoom permet d'indiquer le rapport entre la taille d'arrivée et celle de départ. Ainsi, un facteur de 0.25 indique que la nouvelle image sera quatre fois plus petite que l'image de départ. Pour un facteur de 3, la nouvelle image est 3 fois plus grande que l'image initiale.
 /// \note Attention : le facteur ne doit pas être négatif.
 /// \return Renvoie une nouvelle instance de EZImage allouée dynamiquement (et qui devra être détruite grâce à l'opérateur delete) ou nullptr en cas d’erreur.
 EZImage* scale(double factor) const;
 /// Construit une nouvelle image à partir d'une rotation de l’image courante.
 /// \param theta l'angle de rotation en degrés.
 /// \param quality Si quality = 1, l’algorithme utilisé lisse le résultat (avec une interpolation bilinéaire) ; si quality = 0, l’algorithme privilégie la rapidité (avec un calcul de plus proche voisin), ce qui permet de gagner environ un facteur 3.
 /// \return Une nouvelle instance de EZImage allouée dynamiquement (et qui devra être détruite grâce à l'opérateur delete) ou nullptr en cas d’erreur. La taille de l'image est ajustée pour contenir le résultat.
 /// Dans l’image résultat, le champ has_alpha est mis à 1, et les parties ne provenant pas de l’image source sont transparentes ; le but est qu’elles n’apparaissent pas lors de l’affichage.
 /// \note Le résultat étant indépendant du centre de rotation, il n’est pas demandé en paramètre.
 /// \sa  void rotatePoint(double theta, int src_x, int src_y,int& dst_x, int& dst_y);
 /// \note L’exemple demo++16 illustre les rotations, sans ou avec transparence. Le centre de rotation (croix rouge) est déplaçable avec les flèches. On peut aussi modifier la qualité.
 EZImage* rotate(double theta,int quality) const;
 /// Calcule pour un point de coordonnées src_x,src_y dans l’image source, les nouvelles coordonnées dst_x,dst_y du point correspondant dans l’image résultat.
 /// \param theta l'angle de rotation en degrés.
 /// \param src_x,src_y les coordonnées du point dans l'image de départ.
 /// \param dst_x,dst_y les coordonnées du point dans l'image d'arrivée.
 /// \return Le résultat est obtenu via les paramètres dst_x et dst_y (passés par référence).
 /// \sa EZImage* EZImage::rotate(double theta,int quality) const
 void rotatePoint(double theta, int src_x, int src_y,int& dst_x, int& dst_y);
 friend class EZPixmap;
};

struct sEz_pixmap;

/// La classe EZPixmap permet d'améliorer nettement les performances de tracé à l'écran par rapport à la classe EZImage qui est plus générale mais aussi plus lente.
/// L'affichage d'une instance de EZImage nécessite une série de conversions qui sont faites à chaque fois qu'un tracé est réclamé, ce qui peut prendre plusieurs dizaines de millisecondes. Lorsque l'affichage comporte le dessin d'un nombre important de telles images au sein d'une animation, par exemple dans un jeu vidéo, le résultat peut devenir trop saccadé pour être acceptable.
/// La classe EZPixmap permet de figer une image et de la transférer au plus près du matériel graphique (le plus souvent, jusqu'à la mémoire de la carte graphique), ce qui rendra son tracé très nettement plus rapide.
/// \note L'exemple demo++17 permet de tester la vitesse d’affichage, mesurée en fps (pour frame per second) dans une animation. Utiliser les touches + et - pour modifier le nombre de balles, et la touche p pour activer l’utilisation des pixmaps.
class EZPixmap final {
 struct sEz_pixmap *pixmap;
 EZPixmap()=delete;
public:
 /// Constructeur de la classe EZPixmap à partir d'une instance de la classe EZImage.
 /// \param image L'instance de la classe EZImage à partir de laquelle il faut constuire le EZPixmap.
 /// \note Une fois le EZPixmap construit, il n'a plus aucun lien avec l'instance de EZImage qui a servi à sa construction. En particulier, il n'est pas modifié lorsque cette dernière est modifiée.
 EZPixmap(const EZImage& image);
 /// Destructeur de la classe EZPixmap.
 ~EZPixmap();
 /// Affiche le pixmap dans la fenêtre.
 /// \param win la fenêtre où aura lieu le tracé.
 /// \param x,y les coordonnées du coin supérieur gauche du tracé du pixmap dans la fenêtre.
 void paint(EZWindow& win,int x, int y) const;
 /// Affiche le pixmap dans la fenêtre de manière répétitive dans une zone, comme un papier peint, .
 /// \param win la fenêtre où aura lieu le tracé.
 /// \param x,y les coordonnées du coin supérieur gauche du tracé du pixmap dans la fenêtre.
 /// \param width,height la largeur et la hauteur de la zone dans la fenêtre où le pixmap doit être tracé.
 void tile(EZWindow& win,int x, int y, int width, int height) const;
};

#endif
