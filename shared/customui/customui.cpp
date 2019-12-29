// To use, simply add customui.cpp to your Android.mk, and #include customui.hpp
// CUSTOM UI

#include "../utils/utils.h"
#include "customui.hpp"
using namespace il2cpp_utils;

namespace CustomUI
{

void Init()
{
}

bool TextUI::create()
{

    // gameobj = new GameObject("CustomTextUI");
    log(INFO, "Creating gameObject: %s", name.c_str());
    gameObj = NewUnsafe(GetClassFromName("UnityEngine", "GameObject"), createcsstr(name));

    // gameObj.SetActive(false);
    bool active = false;
    log(INFO, "Setting gameObject.active to false");
    if (!RunMethod(gameObj, "SetActive", &active))
    {
        log(DEBUG, "Failed to set active to false");
        return false;
    }

    // gameObj.AddComponent<TextMeshProUGUI>();
    log(INFO, "Getting type of TMPro.TextMeshProUGUI");
    Il2CppObject *type_tmpugui = il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(GetClassFromName("TMPro", "TextMeshProUGUI")));

    
    log(INFO, "Adding component TMPro.TextMeshProUGUI");
    if (!RunMethod(&textMesh, gameObj, "AddComponent", type_tmpugui))
    {
        log(DEBUG, "Failed to add Component TMPro.TextMeshProUGUI");
        return false;
    }
    // textMesh.font = GameObject.Instantiate(Resources.FindObjectsOfTypeAll<TMP_FontAsset>().First(t => t.name == "Teko-Medium SDF No Glow"));
    log(INFO, "Getting type of TMPro.TMP_FontAsset");
    Il2CppObject *type_fontasset = il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(GetClassFromName("TMPro", "TMP_FontAsset")));
    log(INFO, "Gotten the type!");
    Array<Il2CppObject *> *allObjects;

    // Find Objects of type TMP_fontAsset
    if (!RunMethod(&allObjects, nullptr, GetMethod("UnityEngine", "Resources", "FindObjectsOfTypeAll", 1), type_fontasset))
    {
        // EXCEPTION
        log(DEBUG, "Failed to Find Objects of type TMP_fontAsset");
        return false;
    }
    int match = -1;
    for (int i = 0; i < allObjects->Length(); i++)
    {
        // Treat it as a UnityEngine.Object (which it is) and call the name getter
        Il2CppString *assetName;
        if (!RunMethod(&assetName, allObjects->values[i], "get_name"))
        {
            // EXCEPTION
            log(DEBUG, "Failed to run get_name of assetName");
            return false;
        }
        if (strcmp(to_utf8(csstrtostr(assetName)).c_str(), "Teko-Medium SDF No Glow") == 0)
        {
            // Found matching asset
            match = i;
            break;
        }
    }
    if (match == -1)
    {
        log(CRITICAL, "Could not find matching TMP_FontAsset!");
        return false;
    }

    // Instantiating the font
    log(INFO, "Instantiating the font");
    Il2CppObject *font;
    if (!RunMethod(&font, nullptr, GetMethod("UnityEngine", "Object", "Instantiate", 1), allObjects->values[match]))
    {
        log(DEBUG, "Failed to Instantiate font!");
        return false;
    }

    // Setting the font
    log(INFO, "Setting the font");
    if (!RunMethod(textMesh, "set_font", font))
    {
        log(DEBUG, "Failed to set font!");
        return false;
    }

    // textMesh.rectTransform.SetParent(parent, false);
    log(INFO, "Getting rectTransform");
    Il2CppObject *rectTransform;
    if (!RunMethod(&rectTransform, textMesh, "get_rectTransform"))
    {
        log(DEBUG, "Failed to get rectTransform");
        return false;
    }

    
    log(INFO, "Setting Parent");
    bool value = false;
    if (!RunMethod(rectTransform, "SetParent", parentTransform, &value))
    {
        log(DEBUG, "Failed to set parent!");
        if (parentTransform == nullptr)
        {
            log(DEBUG, "parentTransform is null! Fix it!");
        }
        return false;
    }
    // textMesh.text = text;
    log(INFO, "Setting Text");
    if (!RunMethod(textMesh, "set_text", createcsstr(text)))
    {
        log(DEBUG, "Failed to set text!");
        return false;
    }

    // textmesh.fontSize = fontSize;
    log(INFO, "Setting fontSize");
    if (!RunMethod(textMesh, "set_fontSize", &fontSize))
    {
        log(DEBUG, "Failed to set fontSize!");
        return false;
    }

    // textMesh.color = Color.white;
    log(INFO, "Setting color");
    if (!RunMethod(textMesh, "set_color", &color))
    {
        log(DEBUG, "Failed to set color!");
        return false;
    }
   
    // textMesh.rectTransform.anchorMin = anchorMin
    Vector2 anchorMin = {0.0, 1.0}; //Add to .hpp so it can be changed
    log(INFO, "Setting anchorMin");
    if (!RunMethod(rectTransform, "set_anchorMin", &anchorMin))
    {
        log(DEBUG, "Failed to set anchorMin");
        return false;
    }

    // textMesh.rectTransform.anchorMax = anchorMax
    Vector2 anchorMax = {0.0, 1.0}; //Add to .hpp so it can be changed
    log(INFO, "Setting anchorMax");
    if (!RunMethod(rectTransform, "set_anchorMax", &anchorMax))
    {
        log(DEBUG, "Failed to set anchorMax");
        return false;
    }
    
    // textMesh.rectTransform.sizeDelta = sizeDelta
    log(INFO, "Setting sizeDelta");
    Vector2 sizeDelta = {0.0, 1.0}; //Add to .hpp so it can be changed
    if (!RunMethod(rectTransform, "set_sizeDelta", &sizeDelta))
    {
        log(DEBUG, "Failed to set sizeDelta");
        return false;
    }

    // textMesh.rectTransform.anchoredPosition = anchoredPosition
    Vector2 anchoredPosition = {0.0, 0.0}; //Add to .hpp so it can be changed
    log(INFO, "Setting anchoredPosition");
    if (!RunMethod(rectTransform, "set_anchoredPosition", &anchoredPosition))
    {
        log(DEBUG, "failed to set anchoredPosition");
        return false;
    }

    // gameObj.SetActive(true);
    log(INFO, "Setting gameObject active to true");
    active = true;
    if (!RunMethod(gameObj, "SetActive", &active))
    {
        log(DEBUG, "Failed to set active to true");
        return false;
    }
    log(DEBUG, "Succesfully created gameObj: %s", name.c_str());
    return true;
}

} // namespace CustomUI