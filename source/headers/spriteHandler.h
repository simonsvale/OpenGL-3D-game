#include <vector>
#include <string>

#include <STB/stb_image.h>

using namespace std;

class Sprite
{
    private:
        unsigned char LoadImageTexture();

    public:

// Constructor
        Sprite(string FilePath)
        {
                // Load .atris file, and set the variables.
                LoadSpriteFile(FilePath);
        }


        
// Sprite loading        
        // Load the attribute file for the corrsponding texture, and set the attributes.
        void LoadSpriteFile(string FilePath);

        // The (width, height) of the sprite.
        vector<uint16_t> SpriteSize;



// Collision
        // Determines if the sprite has collision
        bool Collision;
        
        // @NOTE: Perhaps not necesary, since the entire source texture size could be used as the collision box.
        //uint16_t CollisionBox; 


// Animation
        // Determines if the sprite has an animation
        bool HasAnimation;
        
        // The path of the sprite sheet containing the animation frames
        string AnimationSpriteSheetPath;

        // The amount of frames the sprite animation has, max 256, (0 - 255).
        uint8_t AnimationFrameAmount;

// Light
        // 8bit Ambient light value of the sprite, 0 - 255, where 0 is black and 255 is white.
        // @NOTE: Based on the time of day, applies effect onto sprite. (perhaps takes moonlight into count?)
        uint8_t AmbientLightValue;

        // Is sprite lit up by artificial light
        uint8_t ArtificalLightValue = 0;

        // The total light value of the sprite
        uint16_t TotalLightValue;


// Submerged effect
        // Determines if the submerged effect should be applied to the base sprite.              
        // @NOTE: The submerged effect should move the pixels horizontally back and forth.
        // @NOTE: The submerged effect is found in another spritesheet.
        bool HasSubmergedEffect = false;

        // The amount of submerged animation frames.
        uint8_t SubmergedAnimationFrameAmount;

        // Primarily determines the 
        // @NOTE: The Submerged depth, based on the depth multiplies 
        uint8_t SubmergedDepth;

// Wind (Should perhaps be removed and replaced with a sprite on sprite basis? (Difficult to make standard for all sprites, since some objects move more in the wind))
        // Determines if the sprite is affected by wind.
        bool IsWindswept;
        uint8_t WindStrength;


// Render layer, the layer that the sprite should be rendered on.
        uint8_t RenderLayer;


// functions
        // Applies the submerged effect to sprite.
        void ApplySubmergedEffect();
        void ApplyWindEffect();

};


// Lightsource, child of the Sprite class.
class LightSource: public Sprite
{
    public:
        // Determines if the sprite is luminous.
        bool IsLightSource;
        uint8_t LightSourceColour;

        // The radius of the light source
        uint8_t LightSourceRadius;
        
        // The strength of the light source, 
        // @NOTE: 0 = no light / off, determines the rate of the dropoff.
        uint8_t LightSourceStrength;

        // The position of the light source relative to the sprite, where (0, 0) is the top right corner.
        // @NOTE: The last corner is either relative e.i. (1, 1) or determined by the sprite dimensions.
        uint16_t LightSourcePos; // perhaps could be 8bits

        // The type of light source, ex. cone shaped = 1, circular = 0, etc.
        // @NOTE: Multiple could be added, adds the Artifical light Value to nearby sprites, 
        // @NOTE: if two light sources overlap to each other, the strongest should be rendered ontop of the weak.
        uint8_t LightSourceShape;

        // Determines if the light source has an light animation, besides its own animation.
        bool HasLightAnimation;

        // The light cycle animation, based on the light values.
        // @NOTE: if LightAnimationCycle.size() = 0, throw error.
        std::vector<uint8_t> LightAnimationCycle;

        // Function for determining the light source dropoff rate. (should maybe return a vector instead?)
        int CalcLightSourceDropoffRate();

};