macroScript ExportDialog
category: "BoogiePerets tools"
internalCategory: "Export Model Dialog"
buttonText: "BoogiePerets ExportDialog"
tooltip: "Exporter for a3d models. (c) BoogiePerets"
icon:#("BoogiePeretsIcons", 1)
(
	exportModelRollout1 = newRolloutFloater "BoogiePerets a3d scene exporter  v 1.0" 316 206;
	local fout, tabs = ""
	local usedMaterials, usedSubMaterials
		
	rollout exportModelRollout "BoogiePerets a3d scene exporter  v 1.0" width:296 height:176
	(
		label descLabel "Exporter for a3d scenes. (c) BoogiePerets" pos:[9,8] width:261 height:16
		GroupBox grp1 " Export Parameters " pos:[8,32] width:280 height:136
		checkbox exportSelected "Export Only Selected Objects" pos:[16,56] width:192 height:16
		label fileNameLabel "FileName" pos:[16,80] width:56 height:16
		edittext filename "" pos:[11,104] width:205 height:24
		button browseButton "Browse" pos:[224,104] width:60 height:24
		button exportModelButton "Export model" pos:[16,136] width:264 height:24
		
		function getSaveFileStream =
		(
			local f = createFile fileName.text
			if (f == undefined) then
			(
				MessageBox "Couldn't open file for writing !"
				return undefined
			)
			return f
		)
		
		function exportTarget obj = 
		(
			format ( tabs + "name = \"%\"\n") obj.target.name to:fout
			format ( tabs + "transform = %\n") obj.target.transform to:fout
			--format ( tabs + "position += createPos\n") to:ostream
		)
		
		function exportProps obj = 
		(
			for prop in GetPropNames obj do 
			(
				local pname = (prop as String)
				local pval = GetProperty obj pname
				
				-- exclusions
				if (ClassOf obj) == Hedra and prop == #vertices then continue
				
				if( pval == undefined ) then continue
				if( ClassOf pval == string or ClassOf pval == Standardmaterial) then 
					format( tabs + "% % = \"%\"\n" ) (ClassOf pval) pname pval to:fout
				else if( ClassOf pval == BitMap ) then
				(
					local path = (pval as BitMap).fileName
					format( tabs + "% % = \"%\"\n" ) (ClassOf pval) pname path to:fout
				)
				else 
					format( tabs + "% % = %\n" ) (ClassOf pval) pname pval to:fout
			)
		)
		
		function mulVector2Matrix mat vec =
		(
			--vec.x * mat.row1.x + vec.y * mat.row2.x + vec3.z * mat.row3.x + mat.row4.x
			--vec.x * mat.row1.y + vec.y * mat.row2.y + vec3.z * mat.row3.y + mat.row4.y
			--vec.x * mat.row1.z + vec.y * mat.row2.z + vec3.z * mat.row3.z + mat.row4.z
			
			return 
			[vec.x * mat.row1.x + vec.y * mat.row2.x + vec.z * mat.row3.x + mat.row4.x,
			vec.x * mat.row1.y + vec.y * mat.row2.y + vec.z * mat.row3.y + mat.row4.y,
			vec.x * mat.row1.z + vec.y * mat.row2.z + vec.z * mat.row3.z + mat.row4.z]
		)
		
		function exportVertices obj =
		(
			local oldTabs = tabs
			format( tabs + "vertexList\n" ) to:fout
			tabs += "\t"
			
			local matrix3 mat = (inverse obj.transform)
			
			for i = 1 to obj.numVerts do
			(
				--format( tabs + "% %\n") i ((GetVert obj i) - obj.pos) to:fout
				format( tabs + "% %\n") i ( (mulVector2Matrix mat (GetVert obj i))) to:fout
			)
				
			tabs = oldTabs
			format( tabs + "end vertexList\n" ) to:fout
		)
		
		function exportFaces obj =
		(
			local oldTabs = tabs
			
			format( tabs + "faceList\n" ) to:fout
			tabs += "\t"
			
			for i = 1 to obj.numFaces do
			(
				format( tabs + "% %\n") i (GetFace obj i) to:fout
			)
				
			tabs = oldTabs
			format( tabs + "end faceList\n" ) to:fout
			
		)
		
		function exportFaceMatId obj =
		(
			local oldTabs = tabs
			
			format( tabs + "faceMatIDList \n" ) to:fout
			tabs += "\t"
			
			for i = 1 to obj.numFaces do
			(
				format( tabs + "% %\n") i (getFaceMatID obj i) to:fout
			)
				
			tabs = oldTabs
			format( tabs + "end faceMatIDList\n" ) to:fout
		)
		
		function exportTextureCoords obj =
		(
			local oldTabs = tabs
			
			format( tabs + "a3dTextureCoorList\n" ) to:fout
			tabs += "\t"
			
			for i = 1 to obj.numTVerts do
			(
				format( tabs + "% %\n") i (GetTVert obj i) to:fout
			)
				
			tabs = oldTabs
			format( tabs + "end a3dTextureCoorList\n" ) to:fout
		)
		
		function exportNumMapVerts obj =
		(
			local mapChannelCount = meshop.getNumMaps obj
			if( mapChannelCount > 0 and (meshop.getMapSupport obj 1)) then 
				format( tabs + "map1ChannelVertCount = %\n") (meshop.getNumMapVerts obj 1) to:fout
			else
				format( tabs + "map1ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 1 and (meshop.getMapSupport obj 2) ) then 
				format( tabs + "map2ChannelVertCount = %\n") (meshop.getNumMapVerts obj 2) to:fout
			else 
				format( tabs + "map2ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 2 and (meshop.getMapSupport obj 3) ) then 
				format( tabs + "map3ChannelVertCount = %\n") (meshop.getNumMapVerts obj 3) to:fout
			else 
				format( tabs + "map3ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 3 and (meshop.getMapSupport obj 4) ) then 
				format( tabs + "map4ChannelVertCount = %\n") (meshop.getNumMapVerts obj 4) to:fout
			else 
				format( tabs + "map4ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 4 and (meshop.getMapSupport obj 5) ) then 
				format( tabs + "map5ChannelVertCount = %\n") (meshop.getNumMapVerts obj 5) to:fout
			else 
				format( tabs + "map5ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 5 and (meshop.getMapSupport obj 6) ) then 
				format( tabs + "map6ChannelVertCount = %\n") (meshop.getNumMapVerts obj 6) to:fout
			else 
				format( tabs + "map6ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 6 and (meshop.getMapSupport obj 7) ) then 
				format( tabs + "map7ChannelVertCount = %\n") (meshop.getNumMapVerts obj 7) to:fout
			else
				format( tabs + "map7ChannelVertCount = 0\n") to:fout
			if( mapChannelCount > 7 and (meshop.getMapSupport obj 8) ) then 
				format( tabs + "map8ChannelVertCount = %\n") (meshop.getNumMapVerts obj 8) to:fout
			else
				format( tabs + "map8ChannelVertCount = 0\n") to:fout
		)
		
		function exportEmptyMapVerts ind=
		(
			format( tabs+"map%VertexList\n" ) ind to:fout
			format( tabs+"end map%VertexList\n" ) ind to:fout
		)
		
		function exportEmptyMapFaces ind=
		(
			format( tabs+"map%FaceList\n" ) ind to:fout
			format( tabs+"end map%FaceList\n" ) ind to:fout
		)
		
		function exportMapFaces obj ind=
		(
			local oldTabs = tabs
			
			format( tabs+"map%FaceList\n" ) ind to:fout
			tabs += "\t"
		
			local nv = meshop.getNumMapFaces obj ind
			
			for i = 1 to nv do
			(
				format( tabs + "% %\n") i (meshop.getMapFace obj ind i) to:fout
			)
			
			tabs = oldTabs
			format( tabs+"end map%FaceList\n" ) ind to:fout
		)
		
		function exportMapVertsChannel obj ind =
		(
			local oldTabs = tabs
			local nv = meshop.getNumMapVerts obj ind
			
			format( tabs+"map%VertexList\n" ) ind to:fout
			tabs += "\t"
					
			for i = 1 to nv do
			(
				format( tabs + "% %\n") i (meshop.getMapVert obj ind i) to:fout
			)
			
			tabs = oldTabs
			format( tabs+"end map%VertexList\n" ) ind to:fout
		)
		
		function exportMapsVerts obj =
		(
			if( meshop.getMapSupport obj 1 ) then 
				(	exportMapVertsChannel obj 1 )
			else
				( exportEmptyMapVerts 1 )
			
			if( meshop.getMapSupport obj 2 ) then 
				(	exportMapVertsChannel obj 2 )
			else
				( exportEmptyMapVerts 2 )
			if( meshop.getMapSupport obj 3 ) then 
				(	exportMapVertsChannel obj 3 )
			else
				( exportEmptyMapVerts 3 )
			if( meshop.getMapSupport obj 4 ) then 
				(	exportMapVertsChannel obj 4 )
			else
				( exportEmptyMapVerts 4 )
			if( meshop.getMapSupport obj 5 ) then 
				(	exportMapVertsChannel obj 5 )
			else
				( exportEmptyMapVerts 5 )
			if( meshop.getMapSupport obj 6 ) then 
				(	exportMapVertsChannel obj 6 )
				else
				( exportEmptyMapVerts 6 )
			if( meshop.getMapSupport obj 7 ) then 
				(	exportMapVertsChannel obj 7 )
				else
				( exportEmptyMapVerts 7 )
			if( meshop.getMapSupport obj 8 ) then 
				(	exportMapVertsChannel obj 8 )
				else
				( exportEmptyMapVerts 8 )
		)
		
		function exportMapsFaces obj =
		(
			if( meshop.getMapSupport obj 1 ) then 
				(	exportMapFaces obj 1 )
			else
				( exportEmptyMapFaces 1 )
			if( meshop.getMapSupport obj 2 ) then 
				(	exportMapFaces obj 2 )
			else
				( exportEmptyMapFaces 2 )
			if( meshop.getMapSupport obj 3 ) then 
				(	exportMapFaces obj 3 )
			else
				( exportEmptyMapFaces  3 )
			if( meshop.getMapSupport obj 4 ) then 
				(	exportMapFaces obj 4 )
			else
				( exportEmptyMapFaces  4 )
			if( meshop.getMapSupport obj 5 ) then 
				(	exportMapFaces obj 5 )
			else
				( exportEmptyMapFaces  5 )
			if( meshop.getMapSupport obj 6 ) then 
				(	exportMapFaces obj 6 )
				else
				( exportEmptyMapFaces 6 )
			if( meshop.getMapSupport obj 7 ) then 
				(	exportMapFaces obj 7 )
				else
				( exportEmptyMapFaces  7 )
			if( meshop.getMapSupport obj 8 ) then 
				(	exportMapFaces obj 8 )
				else
				( exportEmptyMapFaces  8 )
		)
		
		function exportTransform obj = 
		(
			format ( tabs + "transform = %\n" ) obj.transform to:fout
		)
		
		function exportPivot obj = 
		(
			format ( tabs + "pivot = %\n" ) obj.pivot to:fout
		)
		
		function exportObjectOffsetPos obj = 
		(
			format ( tabs + "objectOffsetPos = %\n" ) obj.objectOffsetPos to:fout
		)
		
		function exportObjectOffsetRot obj = 
		(
			format ( tabs + "objectOffsetRot = %\n" ) obj.objectOffsetRot to:fout
		)
		
		function exportObjectOffsetScale obj = 
		(
			format ( tabs + "objectOffsetScale = %\n" ) obj.objectOffsetScale to:fout
		)
		
		function exportObjectTransform obj = 
		(
			format ( tabs + "objectTransform = %\n" ) obj.objectTransform to:fout
		)
		
		function findMaterial obj =
		(
			if( obj.material == undefined	) then return -1
			for i = 1 to usedMaterials.count do
			(
				if( obj.material == usedMaterials[i] ) then return i
			)
			return -1
		)
		
		function addMaterialName obj =
		(
			if( (findMaterial obj) == -1 ) then 
				usedMaterials[usedMaterials.count + 1] = obj.material
		)
		
		function exportMaterialName obj =
		(
			if( obj.material == undefined ) then 
			( 
				format (tabs + "materialName = \"undefined\"\n") to:fout
			)
			else
			(
				format (tabs + "materialName = \"%\"\n") obj.material.name to:fout
			)
		)
		
				
		function exportSpecificProps obj = 
		(
			-- for furter usage
		)
						
		-- don't change export order!!!!!!!!!!!!!!!
		function exportMesh obj =
		(
			local vs = getUserProp obj "vertexFVF"
			exportSpecificProps obj
			
			format ( tabs + "vertexCount = %\n" ) obj.numVerts to:fout
			format ( tabs + "facesCount = %\n" ) obj.numFaces to:fout
			format ( tabs + "vertexFVF = \"%\"\n" ) vs to:fout
			format ( tabs + "mapChannelCount = %\n") ( (meshop.getNumMaps obj) - 1) to:fout
			exportTransform obj
			exportPivot obj
			exportObjectOffsetPos obj
			exportObjectOffsetRot obj
			exportObjectOffsetScale obj
			exportObjectTransform obj
			exportMaterialName obj
			exportNumMapVerts obj
						
			exportVertices obj
			exportFaces obj
			exportFaceMatId  obj
			--exportTextureCoords obj
			
			exportMapsVerts obj
			exportMapsFaces obj
		
			addMaterialName obj
		)
				
		function exportGeneric obj =
		(
			local oldTabs = tabs
			exportProps obj
			
			if(obj.target != undefined) then 
			(
				format ( tabs + "a3d_target\n" ) to:fout
				tabs += "\t"
				exportTarget obj
				tabs = oldTabs
				format ( tabs + "end a3d_target\n" ) to:fout
			)
		)
		
		function exportOmniLight node = 
		(
			format( tabs + "color = %\n"  ) node.color to:fout
			format( tabs + "position = %\n" ) node.position to:fout
			format( tabs + "farAttenStart = %\n" ) node.farAttenStart to:fout
			format( tabs + "farAttenEnd = %\n" ) node.farAttenEnd to:fout
		)
		
		function exportDirectionalLight node =
		(
			format( tabs + "color = %\n"  ) node.color to:fout
			format( tabs + "direction = %\n" ) node.dir to:fout
		)
		
		function shouldExportMesh obj =
		(
			return obj.numVerts > 0 and obj.numFaces > 0
		)
		
		function exportNode node =
		(
			local nsp = SuperClassOf node
			local nc = ClassOf node
			local oldTabs = tabs
			local res = true
			
			if( node.name == "ModelParameters" or node.name == "SceneParameters" ) then return true
			
			if(nsp == GeometryClass and nc == Editable_mesh and (shouldExportMesh node) != true) then 
			(
				format( "warning mesh % not exported (numVers == 0 or numFaces = 0)\n" ) node.name
				continue
			)
					
			format( tabs + "a3dNode % \"%\"\n" ) nc node.name to:fout
			tabs += "\t"
			
			-- if mesh then export
			if( nsp == GeometryClass and nc == Editable_mesh ) then 
			(
				exportMesh node
			)
			else if( nc == Omnilight ) then 
			(
				exportOmniLight node
			)
			else if( nc == Directionallight ) then 
			(
				exportDirectionalLight node
			)
			else if(not (nc == Targetobject and node.isTarget)) then
			(
				exportGeneric node
			)
			else
			(
				res = false
			)
			tabs = oldTabs
			
			format( tabs + "end a3dNode\n" ) to:fout
			return res
		)
		
		function exportNodeRecurse node =
		(
			if( (exportNode node) == false ) then 
				return false
						
			for child in node.children do
				exportNodeRecurse child
		)
		
		function exportModel =
		(
			format( "a3dModel\n\n" ) to:fout
			for node in rootnode.children do
			(
				exportNodeRecurse node
			)
		)
		
		function exportSelectionModel = 
		(
			format( "a3dModel\n\n" ) to:fout
			for node in selection do
			(
				if( node.isHidden ) then continue
				exportNode node 
			)
		)
		
		function findSubMaterial mat =
		(
			for i = 1 to usedSubMaterials.count do
			(
				if( mat == usedSubMaterials[i] ) then return i
			)
			return -1
		)
		
		function exportMaterialIdList mat =
		(
			for i = 1 to mat.numsubs do
			(
				format( tabs + "%\n" ) mat.materialIDList[i] to:fout
				local smat = (getSubMtl mat i)
				if( (findSubMaterial smat) == -1 ) then 
				( 
					usedSubMaterials[usedSubMaterials.count + 1] = smat
				)
			)
		)
		
		function exportMaterialList mat =
		(
			for i = 1 to mat.numsubs do
			(
				format( tabs + "\"%\"\n" ) mat.materialList[i].name to:fout
			)
		)
		
		function exportMaterialTypeList mat =
		(
			for i = 1 to mat.numsubs do
			(
				format( tabs + "\"%\"\n" ) mat.materialList[i] to:fout
			)
		)
		
		function exportMapEnabled mat =
		(
			for i = 1 to mat.numsubs do
			(
				format( tabs + "%\n" ) mat.mapEnabled [i] to:fout
			)
		)
		
		function exportMultiMaterial mat =
		(
			format( tabs + "name = \"%\"\n" ) mat.name to:fout
			format( tabs + "subMtlCount = %\n" ) mat.numsubs to:fout
			
			--format( tabs + "materialTypeList\n" ) to:fout
			--local oldTabs = tabs
			--tabs += "\t"
			--exportMaterialTypeList mat
			--tabs = oldTabs
			--format( tabs + "end materialTypeList\n" ) to:fout
			
			format( tabs + "materialNameList\n" ) to:fout
			local oldTabs = tabs
			oldTabs = tabs
			tabs += "\t"
			exportMaterialList mat
			tabs = oldTabs
			format( tabs + "end materialNameList\n" ) to:fout
			
			format( tabs + "materialIdList\n" ) to:fout
			oldTabs = tabs
			tabs += "\t"
			exportMaterialIdList mat
			tabs = oldTabs
			format( tabs + "end materialIdList\n" ) to:fout
			
			format( tabs + "mapEnabledList\n" ) to:fout
			oldTabs = tabs
			tabs += "\t"
			exportMapEnabled mat
			tabs = oldTabs
			format( tabs + "end mapEnabledList\n" ) to:fout
		)
		
		function exportDirectX9Shader mat =
		(
			--format (tabs + "effectFile = \"%\"\n") mat.effectFile to:fout
			--format (tabs + "technique = \"%\"\n") mat.technique to:fout
			--format (tabs + "renderMaterial = \"%\"\n") mat.renderMaterial to:fout
			--format (tabs + "diffuseTextureMapChannel = %\n" ) mat.diffuseTextureMapChannel to:fout
			--format (tabs + "lightTextureMapChannel = %\n" ) mat.lightTextureMapChannel to:fout
			--format (tabs + "bumpTextureMapChannel = %\n" ) mat.bumpTextureMapChannel to:fout
			--format (tabs + "opacityTextureMapChannel = %\n" ) mat.opacityTextureMapChannel to:fout
			--format (tabs + "lightDir = %\n") mat.lightDir to:fout
			--format (tabs + "k_a = %\n") mat.k_a to:fout
			--format (tabs + "k_d = %\n") mat.k_d to:fout
			--format (tabs + "k_s = %\n") mat.k_s to:fout
			--format (tabs + "n = %\n") mat.n to:fout
			--format (tabs + "numberOfBitmaps = %") mat.numberofbitmaps() to:fout
			format (tabs + "name = \"%\"\n") mat.name to:fout
			exportProps mat
		)
		
		function exportStandardMaterialMapEnabled mat = 
		(
			local oldTabs = tabs
			format (tabs + "mapEnablesList\n") to:fout
			tabs += "\t"
			for i = 1 to 12 do
			(
				format( tabs + "%\n" ) mat.mapEnables[i] to:fout
			)
			tabs = oldTabs
			format (tabs + "end mapEnablesList\n") to:fout
		)
		
		function exportStandardMaterialMapAmounts mat = 
		(
			local oldTabs = tabs
			format (tabs + "mapAmountsList\n") to:fout
			tabs += "\t"
			for i = 1 to 12 do
			(
				format( tabs + "%\n" ) mat.mapAmounts[i] to:fout
			)
			tabs = oldTabs
			format (tabs + "end mapAmountsList\n") to:fout
		)
		
		function exportMaterialMap map =
		(
			format( tabs + "filename = \"%\"\n" ) map.filename to:fout
			format( tabs + "clipu = %\n" ) map.clipu to:fout
			format( tabs + "clipv = %\n" ) map.clipv to:fout
			format( tabs + "clipw = %\n" ) map.clipw to:fout
			format( tabs + "cliph = %\n" ) map.cliph to:fout
			format( tabs + "umirror = %\n" ) map.coords.U_Mirror to:fout
			format( tabs + "vmirror = %\n" ) map.coords.V_Mirror to:fout
			format( tabs + "utile = %\n" ) map.coords.U_Tile to:fout
			format( tabs + "vtile = %\n" ) map.coords.V_Tile to:fout
			format( tabs + "uvwType= %\n" ) map.coords.UVW_Type to:fout
			format( tabs + "mapChannel = %\n" ) map.coords.mapChannel to:fout
			format( tabs + "uOffset= %\n" ) map.coords.U_Offset to:fout
			format( tabs + "vOffset = %\n" ) map.coords.V_Offset to:fout
			format( tabs + "uTiling= %\n" ) map.coords.U_Tiling to:fout
			format( tabs + "vTiling = %\n" ) map.coords.V_Tiling to:fout
		)
		
		local mapList = #( "Ambient", "Diffuse", "Specular", "SpecularLevel", "Glossiness", "SelfIllumination", "Opacity", "FilterColor", "Bump", "Reflection", "Refraction", "Displacement" )		
		function exportMaterialMaps mat = 
		(
			for i = 1 to 12 do
			(
				if( mat.maps[i] == undefined ) then
				(
					format(tabs + "% = undefined\n") mapList[i] to:fout
					continue
				)
				if( ClassOf mat.maps[i] != BitmapTexture ) then 
				(
					format(tabs + "% = NoBitmapTexture\n") mapList[i] to:fout
					continue
				)
				format( tabs + "% = \"%\"\n" ) mapList[i] mat.maps[i].name to:fout
				local oldTabs = tabs
				tabs += "\t"
				exportMaterialMap mat.maps[i]
				tabs = oldTabs
				format( tabs + "end %\n" ) mapList[i] to:fout
			)
		)
		
		function exportStandardMaterial mat =
		(
			format( tabs + "name = \"%\"\n" ) mat.name to:fout
			format( tabs + "shaderType = %\n" ) mat.shaderType to:fout
			format( tabs + "shaderName = \"%\"\n" ) mat.shaderByName to:fout
			format( tabs + "wire = %\n" ) mat.wire to:fout
			format( tabs + "twoSided = %\n" ) mat.twoSided to:fout
			format( tabs + "faceted = %\n" ) mat.faceted to:fout
			format( tabs + "opacity = %\n" ) mat.opacity to:fout
			format( tabs + "ambient = %\n" ) mat.ambient to:fout
			format( tabs + "diffuse = %\n" ) mat.diffuse to:fout
			format( tabs + "specular = %\n" ) mat.specular to:fout
			format( tabs + "selfIllum = %\n" ) mat.selfIllumColor to:fout
			format( tabs + "specularLevel = %\n" ) mat.specularLevel to:fout
			format( tabs + "glossiness  = %\n" ) mat.glossiness to:fout
			format( tabs + "soften  = %\n" ) mat.soften to:fout
			
			exportStandardMaterialMapEnabled mat
			exportStandardMaterialMapAmounts mat
			exportMaterialMaps mat
		)
				
		function exportMaterial mat = 
		(
			local matClass = ClassOf mat
			local oldTabs = tabs
			if( matClass == Multimaterial ) then 
			(
				format( tabs + "a3dMaterial MultiMaterial\n" ) to:fout
				tabs += "\t"
				exportMultiMaterial mat
				tabs = oldTabs
				format(tabs + "end MultiMaterial\n") to:fout
			)
			else if( matClass == DirectX_9_Shader ) then ( 
				format( tabs + "a3dMaterial DirectX_9_Shader \n" ) to:fout
				tabs += "\t"
				exportDirectX9Shader mat
				tabs = oldTabs
				format(tabs + "end DirectX_9_Shader\n") to:fout
			)
			else if( matClass == Standardmaterial ) then 
			(
				format( tabs + "a3dMaterial StandardMaterial\n" ) to:fout
				tabs += "\t"
				exportStandardMaterial mat
				tabs = oldTabs
				format(tabs + "end StandardMaterial\n") to:fout
			)
		)
		
		function exportMaterials = 
		(
			format( tabs + "materialCount = %\n" ) usedMaterials.count to:fout
			for i = 1 to usedMaterials.count do
			(
				exportMaterial usedMaterials[i]
			)
		)
		
		function exportSubMaterials = 
		(
			format( tabs + "subMaterialCount = %\n" ) usedSubMaterials.count to:fout
			for i = 1 to usedSubMaterials.count do
			(
				exportMaterial usedSubMaterials[i]
			)
		)
		
		function getModelParams =
		(
			-- Select ModelParameters mesh where palced our model parameters
			local modelParams = $ModelParameters
			
			if( modelParams == undefined ) then 
			(
				modelParams = Box()
				modelParams.name = "ModelParameters"
								
				setUserProp modelParams "version" "1.0"
				setUserProp modelParams "exportSelected" false
				setUserProp modelParams "filePath" ""
			
				hide modelParams
			)
			return modelParams
		)
		
		on exportSceneRollout open do
		(
			local modelParams = getModelParams()
			
			if( modelParams == undefined ) then 
			(
				MessageBox "getModelParams() return undefined"
			)
			else
			(			
				exportSelected.checked = getUserProp modelParams "exportSelected"
				fileName.text = getUserProp modelParams "filePath"
			)
		)
		on exportSelected changed state do
		(
			local modelParams = getModelParams()
			if( modelParams == undefined ) then
			(
				MessageBox "getModelParams() return undefined"
			)
			else 
			(
				setUserProp modelParams "exportSelected" exportSelected.checked
			)
		)
		
		function updateFileName txt =
		(
			local modelParams = getModelParams()
			setUserProp modelParams "filePath" txt
		)
		
		on filename entered text do
		(
			updateFileName fileName.text
		)
		
		on browseButton pressed do
		(
			local modelParams = getModelParams
			local savePath = getSaveFileName types: "BoogiePerets Models (*.bpm) |*.bpm|"
			if(savePath != undefined) then 
			(
				fileName.text = savePath
				updateFileName savePath
			)
		)
		on exportModelButton pressed do
		(
			local res = true			
			usedMaterials = #()
			usedSubMaterials = #()
			fout = getSaveFileStream()
			
			if(fout == undefined) then ( MessageBox "getSaveFileStream returns undefined" )			
			
			try(
				if( exportSelected.checked ) then
				(
					exportSelectionModel()
				)
				else
				(
					exportModel()
				)
			
				exportMaterials()
				exportSubMaterials()
			)
			catch(
				format "*** % ***\n" (getCurrentException())
				if( fout != undefined ) then close fout
				res = false
			)
			if( fout != undefined ) then close fout
			
			
			if( res == true ) then MessageBox "export succeded!"
			else MessageBox "export fails!"
		)
	)
	
	addRollout exportModelRollout exportModelRollout1;
)