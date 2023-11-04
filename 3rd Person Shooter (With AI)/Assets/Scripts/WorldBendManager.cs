using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

public class WorldBendManager : MonoBehaviour
{
    private const string bending = "_ENABLE";

    [Range(-0.015f, 0.015f)]
    public float curveAmount;

    public bool showCurveInEditor;

    public Material[] materials;

    #region SetCurve

    private void OnValidate()
    {
        foreach (var mat in materials)
        {
            mat.SetFloat("_CurvedAmount", curveAmount);
        }

        if (Application.isPlaying || showCurveInEditor)
            Shader.EnableKeyword(bending);
        else
            Shader.DisableKeyword(bending);
    }

    #endregion

    #region MonoBehaviour

    private void OnEnable()
    {
        RenderPipelineManager.beginCameraRendering += OnBeginCameraRendering;
        RenderPipelineManager.endCameraRendering += OnEndCameraRendering;
    }

    private void OnDisable()
    {
        RenderPipelineManager.beginCameraRendering -= OnBeginCameraRendering;
        RenderPipelineManager.endCameraRendering -= OnEndCameraRendering;
    }

    #endregion

    #region Methods

    private static void OnBeginCameraRendering(ScriptableRenderContext ctx, Camera cam)
    {
        cam.cullingMatrix = Matrix4x4.Ortho(-99, 99, -99, 99, 0.001f, 99) * cam.worldToCameraMatrix;
    }

    private static void OnEndCameraRendering(ScriptableRenderContext ctx, Camera cam)
    {
        cam.ResetCullingMatrix();
    }

    #endregion
}
