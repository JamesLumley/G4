#include "SpriteManager.h"
#include <hgesprite.h>

SpriteManager::SpriteManager()
{
    // this will not create another HGE, instead we will get the global unique HGE object which is already started
    m_hge = hgeCreate( HGE_VERSION );
}

hgeSprite * SpriteManager::GetSprite( const std::string & name )
{
    // attempt to find the requested texture in cache
    string_to_htexture_map_t::iterator iter = m_tex_map.find( name );
    HTEXTURE t = 0;

    if( iter != m_tex_map.end() ) {
        // use texture from cache
        t = iter->second;
    } else {
        // attempt to load the texture
        t = m_hge->Texture_Load( name.c_str() );
    }
    
    // if loading failed
    if( ! t ) return NULL;

    hgeSprite * spr = new hgeSprite(t, 0.0f, 0.0f, (float)m_hge->Texture_GetWidth(t), (float)m_hge->Texture_GetHeight(t));
    return spr;
}

SpriteManager::~SpriteManager()
{
    // to free all textures we might have loaded during this world
    for( string_to_htexture_map_t::iterator iter = m_tex_map.begin(); iter != m_tex_map.end(); ++iter)
    {
        m_hge->Texture_Free( iter->second );
    }

    m_hge->Release();
}