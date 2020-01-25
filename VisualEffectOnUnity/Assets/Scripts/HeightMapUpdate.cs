using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HeightMapUpdate : MonoBehaviour
{
    public Shader DrawShader;
    public float Size;
    [Range(0,1)]
    public float Strength;
    // Start is called before the first frame update
    RenderTexture heightMapTex;
    Material snowMaterial, drawMaterial;
    public Transform MoveObject;
    int layerMask;
    RaycastHit groundhit;
    void Start()
    {
        drawMaterial = new Material(DrawShader);
        drawMaterial.SetColor("_DrawColor", Color.red);
        layerMask = LayerMask.GetMask("Ground");
        snowMaterial = GetComponent<MeshRenderer>().material;
        print(snowMaterial.name);
        heightMapTex = new RenderTexture(1024, 1024, 0, RenderTextureFormat.ARGBFloat);
        snowMaterial.SetTexture("_HeightTex", heightMapTex);
    }

    // Update is called once per frame
    void Update()
    {
        
        Vector4 position = MoveObject.position;
        Physics.Raycast(position, -Vector3.up, out groundhit, 1f, layerMask);
        drawMaterial.SetVector("_Coordinate", new Vector4(groundhit.textureCoord.x, groundhit.textureCoord.y, 0,0));
        drawMaterial.SetFloat("_Strength", Strength);
        drawMaterial.SetFloat("_Size", Size);
        RenderTexture tmp = RenderTexture.GetTemporary(heightMapTex.width, heightMapTex.height, 0, RenderTextureFormat.ARGBFloat);
        Graphics.Blit(heightMapTex, tmp);
        Graphics.Blit(tmp, heightMapTex, drawMaterial);
        RenderTexture.ReleaseTemporary(tmp);
    }

    private void OnGUI()
    {
        GUI.DrawTexture(new Rect(0, 0, 256, 256), heightMapTex, ScaleMode.ScaleToFit, false, 1);
    }
}
