// ==========================================================================
// SimulationCraft for Star Wars: The Old Republic
// http://code.google.com/p/simulationcraft-swtor/
// ==========================================================================

#include "../simulationcraft.hpp"

struct vanguard_powertech_targetdata_t : public targetdata_t
{
    vanguard_powertech_targetdata_t( player_t* source, player_t* target )
        : targetdata_t( source, target )
    {}
};

void register_vanguard_powertech_targetdata( sim_t* /* sim */ )
{
    // player_type t = SITH_MARAUDER;
    typedef vanguard_powertech_targetdata_t type;

}


// ==========================================================================
// Vanguard / Powertech
// ==========================================================================

struct vanguard_powertech_t : public player_t
{
    // Buffs
    struct buffs_t
    {

    } buffs;

    // Gains
    struct gains_t
    {

    } gains;

    // Procs
    struct procs_t
    {

    } procs;

    // RNGs
    struct rngs_t
    {

    } rngs;

    // Benefits
    struct benefits_t
    {

    } benefits;

    // Cooldowns
    struct cooldowns_t
    {

    } cooldowns;

    // Talents
    struct talents_t
    {
        // Tree 1


        // Tree 2


        // Tree 3

    } talents;

    vanguard_powertech_t( sim_t* sim, player_type pt, const std::string& name, race_type r = RACE_NONE ) :
        player_t( sim, pt == BH_POWERTECH ? BH_POWERTECH : T_VANGUARD, name, ( r == RACE_NONE ) ? RACE_HUMAN : r )
    {


      primary_attribute   = ATTR_STRENGTH;
      secondary_attribute = ATTR_WILLPOWER;

      create_talents();
      create_options();
    }

    // Character Definition
    virtual targetdata_t* new_targetdata( player_t* source, player_t* target ) {return new vanguard_powertech_targetdata_t( source, target );}
    virtual action_t* create_action( const std::string& name, const std::string& options );
    virtual void      init_talents();
    virtual void      init_base();
    virtual void      init_benefits();
    virtual void      init_buffs();
    virtual void      init_gains();
    virtual void      init_procs();
    virtual void      init_rng();
    virtual void      init_actions();
    virtual int       primary_resource() const;
    virtual int       primary_role() const;
    virtual void      create_talents();

    virtual void init_scaling()
    {
      player_t::init_scaling();
      scales_with[ STAT_FORCE_POWER ] = true;
    }
};

namespace { // ANONYMOUS NAMESPACE ==========================================


// ==========================================================================
// Vanguard / Powertech Abilities
// ==========================================================================

struct vanguard_powertech_attack_t : public action_t
{
    vanguard_powertech_attack_t( const char* n, vanguard_powertech_t* p, int r=RESOURCE_NONE, const school_type s=SCHOOL_KINETIC ) :
        action_t( ACTION_ATTACK, n, p, melee_policy, r, s )
    {
        may_crit   = true;
    }

};

struct vanguard_powertech_spell_t : public action_t
{
    vanguard_powertech_spell_t( const char* n, vanguard_powertech_t* p, int r=RESOURCE_NONE, const school_type s=SCHOOL_KINETIC ) :
        action_t( ACTION_ATTACK, n, p, force_policy, r, s )
    {
        may_crit   = true;
        tick_may_crit = true;
    }

};




} // ANONYMOUS NAMESPACE ====================================================

// ==========================================================================
// Vanguard / Powertech Character Definition
// ==========================================================================

// vanguard_powertech_t::create_action ====================================================

action_t* vanguard_powertech_t::create_action( const std::string& name,
                                            const std::string& options_str )
{
    if ( type == BH_POWERTECH )
    {

    }
    else if ( type == T_VANGUARD )
    {

    }

    //if ( name == "apply_charge"           ) return new        apply_charge_t( this, options_str );

    return player_t::create_action( name, options_str );
}

// vanguard_powertech_t::init_talents =====================================================

void vanguard_powertech_t::init_talents()
{
    player_t::init_talents();

    // Darkness|Kinetic Combat
    //talents.thrashing_blades      = find_talent( "Thrashing Blades" );

    // Deception|Infiltration

    // Madness|Balance
}

// vanguard_powertech_t::init_base ========================================================

void vanguard_powertech_t::init_base()
{
    player_t::init_base();

    default_distance = 3;
    distance = default_distance;

}

// vanguard_powertech_t::init_benefits =======================================================

void vanguard_powertech_t::init_benefits()
{
    player_t::init_benefits();

}

// vanguard_powertech_t::init_buffs =======================================================

void vanguard_powertech_t::init_buffs()
{
    player_t::init_buffs();

    // buff_t( player, name, max_stack, duration, cd=-1, chance=-1, quiet=false, reverse=false, rng_type=RNG_CYCLIC, activated=true )
    // buff_t( player, id, name, chance=-1, cd=-1, quiet=false, reverse=false, rng_type=RNG_CYCLIC, activated=true )
    // buff_t( player, name, spellname, chance=-1, cd=-1, quiet=false, reverse=false, rng_type=RNG_CYCLIC, activated=true )

    //bool is_powertech = ( type == BH_POWERTECH );



}

// vanguard_powertech_t::init_gains =======================================================

void vanguard_powertech_t::init_gains()
{
    player_t::init_gains();

}

// vanguard_powertech_t::init_procs =======================================================

void vanguard_powertech_t::init_procs()
{
    player_t::init_procs();

}

// vanguard_powertech_t::init_rng =========================================================

void vanguard_powertech_t::init_rng()
{
    player_t::init_rng();

}

// vanguard_powertech_t::init_actions =====================================================

void vanguard_powertech_t::init_actions()
{
    //======================================================================================
    //
    //   Please Mirror all changes between Jedi Shadow and Sith Assassin!!!
    //
    //======================================================================================

    if ( action_list_str.empty() )
    {
        if ( type == T_VANGUARD )
        {
            action_list_str += "stim,type=exotech_resolve";
            action_list_str += "/snapshot_stats";

            switch ( primary_tree() )
            {


            default: break;
            }

            action_list_default = 1;
        }

        // BH_POWERTECH
        else
        {
            action_list_str += "stim,type=exotech_resolve";
            action_list_str += "/snapshot_stats";

            switch ( primary_tree() )
            {

            default: break;
            }

            action_list_default = 1;
        }
    }

    player_t::init_actions();
}

// vanguard_powertech_t::primary_resource ==================================================

int vanguard_powertech_t::primary_resource() const
{
    return RESOURCE_FORCE;
}

// vanguard_powertech_t::primary_role ==================================================

int vanguard_powertech_t::primary_role() const
{
    switch ( player_t::primary_role() )
    {
    case ROLE_TANK:
        return ROLE_TANK;
    case ROLE_DPS:
    case ROLE_HYBRID:
        return ROLE_HYBRID;
    default:

        break;
    }

    return ROLE_HYBRID;
}

// vanguard_powertech_t::create_talents ==================================================

void vanguard_powertech_t::create_talents()
{
#if 0
    static const struct
    {
        const char* name;
        int maxpoints;
    } talent_descriptions[] = {
        // Darkness|Kinetic Combat
        { "Thrashing Blades", 2 }, { "Lightning Reflexes", 2 }, { "Charge Mastery", 3 },
        { "Shroud of Darkness", 3 }, { "Lightning Recovery", 2 }, { "Swelling Shadows", 2 }, { "Electric Execution", 3 },
        { "Disjunction", 1 }, { "Energize", 1 }, { "Dark Ward", 1 }, { "Premonition", 2 },
        { "Hollow", 2 }, { "Blood of Sith", 3 },
        { "Electrify", 1 }, { "Eye of the Storm", 1 }, { "Force Pull", 1 }, { "Nerve Wracking", 3 },
        { "Harnessed Darkness", 2 }, { "Mounting Darkness", 3 },
        { "Wither", 1 },
        { 0, 0 },

        // Deception|Infiltration
        { "Insulation", 2 }, { "Duplicity", 3 }, { "Dark Embrace", 2 },
        { "Obfuscation", 3 }, { "Recirculation", 2 }, { "Avoidance", 2 },
        { "Induction", 2 }, { "Surging Charge", 1 }, { "Darkswell", 1 }, { "Deceptive Power", 1 },
        { "Entropic Field", 2 }, { "Saber Conduit", 3 }, { "Fade", 2 }, { "Static Cling", 2 },
        { "Resourcefulness", 2 }, { "Static Charges", 2 }, { "Low Slash", 1 },
        { "Crackling Blasts", 5 },
        { "Voltaic Slash", 1 },
        { 0, 0 },

        // Madness|Balance
        { "Exploitive Strikes", 3 }, { "Sith Defiance", 2 }, { "Crackling Charge", 2 },
        { "Oppressing Force", 2 }, { "Chain Shock", 3 }, { "Parasitism", 2 }, { "Torment", 2 },
        { "Death Field", 1 }, { "Fanaticism", 2 }, { "Claws of Decay", 2 },
        { "Haunted Dreams", 2 }, { "Corrupted Flesh", 2 }, { "Raze", 1 },
        { "Deathmark", 1 }, { "Calculating Mind", 2 }, { "Unearthed Knowledge", 2 },
        { "Creeping Death", 3 }, { "Devour", 2 },
        { "Creeping Terror", 1 },
        { 0, 0 },

    };

    unsigned i = 0;
    for ( unsigned tree = 0; tree < 3; ++tree )
    {
        for(; talent_descriptions[ i ].name != 0; ++i )
        {
            talent_trees[ tree ].push_back( new talent_t( this, talent_descriptions[ i ].name, tree,
                                                          talent_descriptions[ i ].maxpoints ) );
        }
        ++i;
    }
#endif
    player_t::create_talents();
}

// ==========================================================================
// PLAYER_T EXTENSIONS
// ==========================================================================

// player_t::create_vanguard ============================================

player_t* player_t::create_vanguard( sim_t* sim, const std::string& name, race_type r )
{
    return new vanguard_powertech_t( sim, T_VANGUARD, name, r );
}

// player_t::create_powertech ==========================================

player_t* player_t::create_powertech( sim_t* sim, const std::string& name, race_type r )
{
    return new vanguard_powertech_t( sim, BH_POWERTECH, name, r );
}

// player_t::vanguard_powertech_init ===========================================

void player_t::vanguard_powertech_init( sim_t* /* sim */ )
{

}

// player_t::vanguard_powertech_combat_begin ===================================

void player_t::vanguard_powertech_combat_begin( sim_t* /* sim */ )
{

}