
#include "qjsonmodel.h"

#include "newitemwidget.h"
#include "ui_newitemwidget.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::NewItemWidget Constructor.
 */
NewItemWidget::NewItemWidget( QWidget* parent )
	: QWidget( parent )
	, m_ui( new Ui::NewItemWidget )
{
	m_ui->setupUi( this );

	setWindowFlag( Qt::MSWindowsFixedSizeDialogHint );

	m_itemClass[ ARMOR ]	= "com.shatteredpixel.shatteredpixeldungeon.items.armor.";
	m_itemClass[ ARTIFACT ]	= "com.shatteredpixel.shatteredpixeldungeon.items.artifacts.";
	m_itemClass[ BAG ]		= "com.shatteredpixel.shatteredpixeldungeon.items.bags.";
	m_itemClass[ POTION ]	= ""; // special case
	m_itemClass[ RING ]		= "com.shatteredpixel.shatteredpixeldungeon.items.rings.";
	m_itemClass[ SCROLL ]	= "com.shatteredpixel.shatteredpixeldungeon.items.scrolls.";
	m_itemClass[ WAND ]		= "com.shatteredpixel.shatteredpixeldungeon.items.wands.";
	m_itemClass[ WEAPON ]	= "com.shatteredpixel.shatteredpixeldungeon.items.weapon.melee.";

	m_itemTypes[ ARMOR ]	= "Armor";
	m_itemTypes[ ARTIFACT ]	= "Artifact";
	m_itemTypes[ BAG ]		= "Bag";
	m_itemTypes[ POTION ]	= "Potion";
	m_itemTypes[ RING ]		= "Ring";
	m_itemTypes[ SCROLL ]	= "Scroll";
	m_itemTypes[ WAND ]		= "Wand";
	m_itemTypes[ WEAPON ]	= "Weapon";

	QMap< int, QString >	armors;
	armors[ A_CLOTH ]	= "ClothArmor";
	armors[ A_LEATHER ]	= "LeatherArmor";
	armors[ A_MAIL ]	= "MailArmor";
	armors[ A_SCALE ]	= "ScaleArmor";
	armors[ A_PLATE ]	= "PlateArmor";
	m_items[ ARMOR ]	= armors;

	QMap< int, QString >	artifact;
	artifact[ AR_ALCHEMIST_TOOLKIT ]		= "AlchemistsToolkit";
	artifact[ AR_CAPE_OF_THORNS ]			= "CapeOfThorns";
	artifact[ AR_CHALICE_OF_BLOOD ]			= "ChaliceOfBlood";
	artifact[ AR_CLOACK_OF_SHADOWS ]		= "CloackOfShadows";
	artifact[ AR_DRIED_ROSE ]				= "DriedRose";
	artifact[ AR_ETHEREAL_CHAINS ]			= "EthernalChains";
	artifact[ AR_HORN_OF_PLENTY ]			= "HornOfPlenty";
	artifact[ AR_LLOYDS_BEACON ]			= "LloydsBeacon";
	artifact[ AR_MASTER_THIEVES_ARMBAND ]	= "MasterThievesArmband";
	artifact[ AR_SANDALS_OF_NATURE ]		= "SandalsOfNature";
	artifact[ AR_TALISMAN_OF_FORESIGHT ]	= "TalismanOfForesight";
	artifact[ AR_TIMEKEEPERS_HOURGLASS ]	= "TimekeepresHourglass";
	artifact[ AR_UNSTABLE_SPELLBOOK ]		= "UnstableSpellbook";
	m_items[ ARTIFACT ]	= artifact;

	QMap< int, QString >	bags;
	bags[ B_MAGICAL_HOLSTER ]	= "MagicalHolster";
	bags[ B_POTION_BANDOLIER ]	= "PotionBandolier";
	bags[ B_SCROLL_HOLSTER ]	= "ScrollHolder";
	bags[ B_VELVET_POUCH ]		= "VelvetPouch";
	m_items[ BAG ]	= bags;

	QMap< int, QString >	potions;
	potions[ P_EXPERIENCE ]		= "PotionOfExperience";
	potions[ P_FROST ]			= "PotionOfFrost";
	potions[ P_HASTE ]			= "PotionOfHaste";
	potions[ P_HEALING ]		= "PotionOfHealing";
	potions[ P_INVISIBILITY ]	= "PotionOfInvisibility";
	potions[ P_LEVITATION ]		= "PotionOfLevitation";
	potions[ P_LIQUID_FLAME ]	= "PotionOfLiquidFlame";
	potions[ P_MIND_VISION ]	= "PotionOfMindVision";
	potions[ P_PARALYTIC_GAS ]	= "PotionOfParalyticGas";
	potions[ P_PURITY ]			= "PotionOfPurity";
	potions[ P_STRENGTH ]		= "PotionOfStrength";
	potions[ P_TOXIC_GAS ]		= "PotionOfToxicGas";

	// brews
	potions[ P_BLIZZARD ]	= "BlizzardBrew";
	potions[ P_CAUSTIC ]	= "CausticBrew";
	potions[ P_FRIGID ]		= "FrigidBrew";
	potions[ P_FROSTFIRE ]	= "FrostfireBrew";
	potions[ P_INFERNAL ]	= "InfernalBrew";
	potions[ P_SHOCKING ]	= "ShockingBrew";
	potions[ P_WICKED ]		= "WickedBrew";

	// elixirs
	potions[ P_AQUATIC_REJUVENATION ]	= "ElixirOfAquaticRejuvenation";
	potions[ P_DRAGONS_BLOOD ]			= "ElixirOfDragonsBlood";
	potions[ P_HONEYED_HEALING ]		= "ElixirOfHoneyedHealing";
	potions[ P_ICY_TOUCH ]				= "ElixirOfIcyTouch";
	potions[ P_MIGHT ]					= "ElixirOfMight";
	potions[ P_RESTORATION ]			= "ElixirOfRestoration";
	potions[ P_TOXIC_ESSENSE ]			= "ElixirOfToxicEssense";
	potions[ P_VITALITY ]				= "ElixirOfVitality";

	// exotic
	potions[ P_ADRENALINE_SURGE ]	= "PotionOfAdrenalineSurge";
	potions[ P_CLEANSING ]			= "PotionOfCleansing";
	potions[ P_CORROSIVE_GAS ]		= "PotionOfCorrosiveGas";
	potions[ P_DRAGONS_BREATH ]		= "PotionOfDragonsBreath";
	potions[ P_EARTHEN_ARMOR ]		= "PotionOfEarthenArmor";
	potions[ P_HOLY_FUROR ]			= "PotionOfHolyFuror";
	potions[ P_MAGICAL_SIGHT ]		= "PotionOfMagicalSight";
	potions[ P_SHIELDING ]			= "PotionOfShielding";
	potions[ P_SHROUDING_FOG ]		= "PotionOfShroudingFog";
	potions[ P_SNAP_FREEZE ]		= "PotionOfSnapFreeze";
	potions[ P_STAMINA ]			= "PotionOfStamina";
	potions[ P_STORM_CLOUDS ]		= "PotionOfStormClouds";
	m_items[ POTION ]	= potions;

	QMap< int, QString >	rings;
	rings[ R_ACCURACY ]			= "RingOfAccuracy";
	rings[ R_ELEMENTS ]			= "RingOfElements";
	rings[ R_ENERGY ]			= "RingOfEnergy";
	rings[ R_FORCE ]			= "RingOfForce";
	rings[ R_FUROR ]			= "RingOfFuror";
	rings[ R_HASTE ]			= "RingOfHaste";
	rings[ R_MIGHT ]			= "RingOfMight";
	rings[ R_SHARPSHOOTING ]	= "RingOfSharpshooting";
	rings[ R_TENACITY ]			= "RingOfTenacity";
	rings[ R_WEALTH ]			= "RingOfWealth";
	m_items[ RING ]	= rings;

	QMap< int, QString >	scrolls;
	scrolls[ S_IDENTIFY ]		= "ScrollOfIdentify";
	scrolls[ S_LULLABY ]		= "ScrollOfLullaby";
	scrolls[ S_MAGIC_MAPPING ]	= "ScrollOfMagicMapping";
	scrolls[ S_MIRROR_IMAGE ]	= "ScrollOfMirrorImage";
	scrolls[ S_RAGE ]			= "ScrollOfRage";
	scrolls[ S_RECHARGING ]		= "ScrollOfRecharging";
	scrolls[ S_REMOVE_CURSE ]	= "ScrollOfRemoveCurse";
	scrolls[ S_RETRIBUTION ]	= "ScrollOfRetribution";
	scrolls[ S_TELEPORTATION ]	= "ScrollOfTeleportation";
	scrolls[ S_TERROR ]			= "ScrollOfTerror";
	scrolls[ S_TRANSMUTATION ]	= "ScrollOfTransmutation";
	scrolls[ S_UPGRADE ]		= "ScrollOfUpgrade";
	m_items[ SCROLL ]	= scrolls;

	QMap< int, QString >	wands;
	wands[ WA_BLAST_WAVE ]		= "WandOfBlastWave";
	wands[ WA_CORROSION ]		= "WandOfCorrosion";
	wands[ WA_CORRUPTION ]		= "WandOfCorruption";
	wands[ WA_DISINTEGRATION ]	= "WandOfDisintegration";
	wands[ WA_FIREBLAST ]		= "WandOfFireblast";
	wands[ WA_FROST ]			= "WandOfFrost";
	wands[ WA_LIGHTNING ]		= "WandOfLightning";
	wands[ WA_MAGIC_MISSILE ]	= "WandOfMagicMissile";
	wands[ WA_PRISMATIC_LIGHT ]	= "WandOfPrismaticLight";
	wands[ WA_REGROWTH ]		= "WandOfRegrowth";
	wands[ WA_TRANSMUTATION ]	= "WandOfTransfusion";
	m_items[ WAND ]	= wands;

	QMap< int, QString >	weapons;
	weapons[ W_ASSASINS_BLADE ]		= "AssassinsBlade";
	weapons[ W_BATTLE_AXE ]			= "BattleAxe";
	weapons[ W_CROSSBOW ]			= "Crossbow";
	weapons[ W_DAGGER ]				= "Dagger";
	weapons[ W_DIRK ]				= "Dirk";
	weapons[ W_FLAIL ]				= "Flail";
	weapons[ W_GAUNTLET ]			= "Gauntlet";
	weapons[ W_GLAIVE ]				= "Glaive";
	weapons[ W_GLOVES ]				= "Gloves";
	weapons[ W_GREATAXE ]			= "Greataxe";
	weapons[ W_GREATSHIELD ]		= "Greatshield";
	weapons[ W_GREATSWORD ]			= "Greatsword";
	weapons[ W_HAND_AXE ]			= "HandAxe";
	weapons[ W_LONGSWORD ]			= "Longsword";
	weapons[ W_MACE ]				= "Mace";
	weapons[ W_QUARTERSTAFF ]		= "Quarterstaff";
	weapons[ W_ROUND_SHIELD ]		= "RoundShield";
	weapons[ W_RUNIC_BLADE ]		= "RunicBlade";
	weapons[ W_SAI ]				= "Sai";
	weapons[ W_SCIMITAR ]			= "Scimitar";
	weapons[ W_SHORTSWORD ]			= "Shortsword";
	weapons[ W_SPEAR ]				= "Spear";
	weapons[ W_SWORD ]				= "Sword";
	weapons[ W_WAR_HAMMER ]			= "WarHammer";
	weapons[ W_WHIP ]				= "Whip";
	weapons[ W_WORN_SHORTSWORD ]	= "WornShortsword";
	m_items[ WEAPON ]	= weapons;


	m_weaponCurses[ WC_NONE ]			= "None";
	m_weaponCurses[ WC_ANNOYING ]		= "Annoying";
	m_weaponCurses[ WC_DISPLACING ]		= "Displacing";
	m_weaponCurses[ WC_EXHAUSTING ]		= "Exhausting";
	m_weaponCurses[ WC_FRAGILE ]		= "Fragile";
	m_weaponCurses[ WC_FRIENDLY ]		= "Friendly";
	m_weaponCurses[ WC_POLARIZED ]		= "Polarized";
	m_weaponCurses[ WC_SACRIFICIAL ]	= "Sacrificial";
	m_weaponCurses[ WC_WAYWARD ]		= "Wayward";

	m_weaponEnchantments[ WE_NONE ]		= "None";
	m_weaponEnchantments[ WE_BLAZING ]		= "Blazing";
	m_weaponEnchantments[ WE_BLOCKING ]		= "Blocking";
	m_weaponEnchantments[ WE_BLOOMING ]		= "Blooming";
	m_weaponEnchantments[ WE_CHILLING ]		= "Chilling";
	m_weaponEnchantments[ WE_ELASTIC ]		= "Elastic";
	m_weaponEnchantments[ WE_GRIM ]			= "Grim";
	m_weaponEnchantments[ WE_LUCKY ]		= "Lucky";
	m_weaponEnchantments[ WE_PRECISE ]		= "Precise";
	m_weaponEnchantments[ WE_PROJECTING ]	= "Projecting";
	m_weaponEnchantments[ WE_SHOCKING ]		= "Shocking";
	m_weaponEnchantments[ WE_SWIFT ]		= "Swift";
	m_weaponEnchantments[ WE_UNSTABLE ]		= "Unstable";
	m_weaponEnchantments[ WE_VAMPIRIC ]		= "Vampiric";

	m_armorCurses[ AC_NONE ]			= "None";
	m_armorCurses[ AC_ANTI_ENTROPY ]	= "AntiEntropy";
	m_armorCurses[ AC_BULK ]			= "Bulk";
	m_armorCurses[ AC_CORROSION ]		= "Corrosion";
	m_armorCurses[ AC_DISPLACEMENT ]	= "Displacement";
	m_armorCurses[ AC_METABOLISM ]		= "Metabolism";
	m_armorCurses[ AC_MULTIPLICITY ]	= "Multiplicity";
	m_armorCurses[ AC_OVERGROWTH ]		= "Overgrowth";
	m_armorCurses[ AC_STENCH ]			= "Stench";

	m_armorGlyphs[ AE_NONE ]			= "None";
	m_armorGlyphs[ AE_AFFECTION ]		= "Affection";
	m_armorGlyphs[ AE_ANTI_MAGIC ]		= "AntiMagic";
	m_armorGlyphs[ AE_BRIMSTONE ]		= "Brimstone";
	m_armorGlyphs[ AE_CAMOUFLAGE ]		= "Camouflage";
	m_armorGlyphs[ AE_ENTANGLEMENT ]	= "Entanglement";
	m_armorGlyphs[ AE_FLOW ]			= "Flow";
	m_armorGlyphs[ AE_OBFUSCATION ]		= "Obfuscation";
	m_armorGlyphs[ AE_POTENTIAL ]		= "Potential";
	m_armorGlyphs[ AE_REPULSION ]		= "Repulsion";
	m_armorGlyphs[ AE_STONE ]			= "Stone";
	m_armorGlyphs[ AE_SWIFTNESS ]		= "Swiftness";
	m_armorGlyphs[ AE_THORNS ]			= "Thorns";
	m_armorGlyphs[ AE_VISCOSITY ]		= "Viscosity";

	setupItemComboBox();

	auto	intValidator	= new QIntValidator( 0, 10, this );
	m_ui->m_leLevel->setValidator( intValidator );
	m_ui->m_leQuantity->setValidator( intValidator );

	connect( m_ui->m_cbItem, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onItemComboBoxIndexChanged( int ) ) );
	connect( m_ui->m_btnDone, SIGNAL( clicked( bool ) ), this, SLOT( onDoneBtnClicked( bool ) ) );
	connect( m_ui->m_btnCancel, SIGNAL( clicked( bool ) ), this, SLOT( onCancelBtnClicked( bool ) ) );
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::~NewItemWidget Destructor.
 */
NewItemWidget::~NewItemWidget()
{
	delete	m_ui;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::setupItemComboBox A method that setups the item combo
 * box.
 */
void	NewItemWidget::setupItemComboBox()
{
	m_ui->m_cbItem->clear();
	for( auto item : m_itemTypes )
	{
		m_ui->m_cbItem->addItem( item );
	}

	setupTypeComboBox();
	setupCurseAndEnchantmentComboBox();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::setupTypeComboBox	A method that setups the item type
 * combo box.
 */
void	NewItemWidget::setupTypeComboBox()
{
	int	item	= m_ui->m_cbItem->currentIndex();

	m_ui->m_cbType->clear();
	for( auto item : m_items[ item ] )
	{
		m_ui->m_cbType->addItem( item );
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::setupCurseAndEnchantmentComboBox	A method that setups
 * the enchantment and curse for the item.
 */
void	NewItemWidget::setupCurseAndEnchantmentComboBox()
{
	int	itemType	= m_ui->m_cbItem->currentIndex();
	m_ui->m_cbCurse->clear();
	m_ui->m_cbEnchantment->clear();

	switch( itemType )
	{
		case	WAND:
		case	ARTIFACT:
		case	RING:
			m_ui->m_cbCurse->addItem( "false" );
			m_ui->m_cbCurse->addItem( "true" );

			m_ui->m_cbEnchantment->setEnabled( false );
			m_ui->m_cbCurse->setEnabled( true );
			break;

		case	WEAPON:

			for( auto el : m_weaponCurses )
				m_ui->m_cbCurse->addItem( el );

			for( auto el : m_weaponEnchantments )
				m_ui->m_cbEnchantment->addItem( el );

			m_ui->m_cbEnchantment->setEnabled( true );
			m_ui->m_cbCurse->setEnabled( true );
			break;

		case	ARMOR:
			for( auto el : m_armorCurses )
				m_ui->m_cbCurse->addItem( el );

			for( auto el : m_armorGlyphs )
				m_ui->m_cbEnchantment->addItem( el );

			m_ui->m_cbEnchantment->setEnabled( true );
			m_ui->m_cbCurse->setEnabled( true );
			break;

		case	BAG:
		case	POTION:
		case	SCROLL:
		default:
			m_ui->m_cbCurse->setEnabled( false );
			m_ui->m_cbEnchantment->setEnabled( false );
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::createJsonItem A helper method that constructs json
 * item with the given parameters.
 */
QJsonTreeItem*	NewItemWidget::createJsonItem( const QString& itemClass, int type, int itemLevel, int quantity, const QString& enchantment, const QString& curse )
{
	int				level		= itemLevel;
	QJsonTreeItem*	itemRoot	= new QJsonTreeItem();
	itemRoot->setType( QJsonValue::Object );

	auto	addField	= [itemRoot]( const QString& key, const QString& value )
	{
		QJsonTreeItem*	tempItem	= new QJsonTreeItem( itemRoot );
		tempItem->setKey( key );
		tempItem->setValue( value );

		itemRoot->appendChild( tempItem );
	};

	auto	addEnchantment	= [itemRoot]( const QString& enchLabel, const QString& className )
	{
		QJsonTreeItem*	tempItem	= new QJsonTreeItem( itemRoot );
		tempItem->setKey( enchLabel );
		tempItem->setType( QJsonValue::Object );

		QJsonTreeItem*	itemCurse	= new QJsonTreeItem( tempItem );
		itemCurse->setKey( "__className" );
		itemCurse->setValue( className );

		tempItem->appendChild( itemCurse );
		itemRoot->appendChild( tempItem );
	};


	addField( "__className", itemClass );
	addField( "levelKnown", "true" );

	switch( type )
	{
		case	RING:
			addField( "level_to_ID", "0" );
			quantity	= 1;
			break;

		case	WAND:
			addField( "curCharges", "1" );
			addField( "available_uses", "1" );
			addField( "curChargesKnown", "1" );

		[[clang::fallthrough]];
		case	WEAPON:
		case	ARMOR:
			quantity	= 1;
			addField( "augment", "NONE" );
			addField( "uses_left_to_id",  "1" );
			addField( "available_uses", "5" );
			addField( "levelKnown", "true" );
			break;

		case	BAG:
		{
			QJsonTreeItem*	tempItem	= new QJsonTreeItem( itemRoot );
			tempItem->setKey( "inventory" );
			tempItem->setType( QJsonValue::Array );

			itemRoot->appendChild( tempItem );
		}

		[[clang::fallthrough]];
		default:
			level	= 0;
			break;
	}

	QMap< int, QString >	itemEnchantmentType;
	itemEnchantmentType[ WEAPON ]	= "enchantment";
	itemEnchantmentType[ ARMOR ]	= "glyph";

	bool	isCursed	= false;
	switch( type )
	{
		case	ARMOR:
		case	WEAPON:
			if( m_ui->m_cbCurse->currentIndex() != 0 )
			{
				isCursed	= true;
				addEnchantment( itemEnchantmentType[ type ], curse );
				break;
			}

			if( m_ui->m_cbEnchantment->currentIndex() != 0 )
			{
				addEnchantment( itemEnchantmentType[ type ], enchantment );
				break;
			}

			break;

		case	RING:
		case	ARTIFACT:
		case	WAND:
			if( m_ui->m_cbCurse->currentText() == "true" )
				isCursed	= true;
			break;

		default:
			break;
	}

	addField( "cursed", isCursed ? "true" : "false" );
	addField( "cursedKnown", "true" );

	addField( "level", QString::number( level ) );
	addField( "quantity", QString::number( quantity ) );

	return	itemRoot;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::onItemComboBoxIndexChanged	A slot that is called when
 * the item combo box's index has changed.
 */
void	NewItemWidget::onItemComboBoxIndexChanged( int index )
{
	Q_UNUSED( index );

	setupTypeComboBox();
	setupCurseAndEnchantmentComboBox();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::onDoneBtnClicked	A slot that is called when when the
 * done button is clicked.
 */
void	NewItemWidget::onDoneBtnClicked( bool clicked )
{
	Q_UNUSED( clicked );

	QString	itemEnchantment;
	QString	itemCurse;
	QString	itemClass;
	int		level		= 1;
	int		quantity	= 1;

	QString	levelText	= m_ui->m_leLevel->text();
	if( ! levelText.isEmpty() )
		level	= levelText.toInt();

	QString	quantityText	= m_ui->m_leQuantity->text();
	if( ! quantityText.isEmpty() )
		quantity	= quantityText.toInt();

	int	itemIndex	= m_ui->m_cbItem->currentIndex();
	int	typeIndex	= m_ui->m_cbType->currentIndex();
	itemClass	= m_itemClass[ itemIndex ];

	switch( itemIndex )
	{
		case	WEAPON:
			itemCurse		= "com.shatteredpixel.shatteredpixeldungeon.items.weapon.curses." + m_ui->m_cbCurse->currentText();
			itemEnchantment	= "com.shatteredpixel.shatteredpixeldungeon.items.weapon.enchantments." + m_ui->m_cbEnchantment->currentText();
			quantity		= 1;
			break;

		case	ARMOR:
			itemCurse		= "com.shatteredpixel.shatteredpixeldungeon.items.armor.curses." + m_ui->m_cbCurse->currentText();
			itemEnchantment	= "com.shatteredpixel.shatteredpixeldungeon.items.armor.glyphs." + m_ui->m_cbEnchantment->currentText();;
			quantity		= 1;
			break;

		case	ARTIFACT:
		case	RING:
		case	WAND:
			itemCurse	= m_ui->m_cbCurse->currentText();
			quantity	= 1;
			break;


		case	BAG:
			quantity	= 1;
			level		= 0;
			break;

		case	POTION:
			if( typeIndex >= P_EXPERIENCE && typeIndex < P_BLIZZARD )
			{
				itemClass	= "com.shatteredpixel.shatteredpixeldungeon.items.potions.";
			}

			if( typeIndex >= P_BLIZZARD && typeIndex < P_AQUATIC_REJUVENATION )
			{
				itemClass	= "com.shatteredpixel.shatteredpixeldungeon.items.potions.brews.";
			}

			if( typeIndex >= P_AQUATIC_REJUVENATION && typeIndex < P_ADRENALINE_SURGE )
			{
				itemClass	= "com.shatteredpixel.shatteredpixeldungeon.items.potions.elixirs.";
			}

			if( typeIndex >= P_ADRENALINE_SURGE )
			{
				itemClass	= "com.shatteredpixel.shatteredpixeldungeon.items.potions.exotic.";
			}

		[[clang::fallthrough]]; case	SCROLL:
			level	= 0;
			break;
	}

	itemClass	+= m_items[ itemIndex ][ typeIndex ];

	QJsonTreeItem*	item	= createJsonItem( itemClass, itemIndex, level, quantity, itemEnchantment, itemCurse );

	emit	addItem( item, typeIndex );
	close();
}
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief NewItemWidget::onCancelBtnClicked A slot that is called when the
 * cancel button is clicked.
 */
void	NewItemWidget::onCancelBtnClicked( bool clicked )
{
	Q_UNUSED( clicked );

	close();
}
////////////////////////////////////////////////////////////////////////////////
