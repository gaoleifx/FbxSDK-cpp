#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")

int numTabs = 0;

void printTabs()
{
	for (int i = 0; i < numTabs; i++)
	{
		printf("\t");
	}
}

FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case FbxNodeAttribute::eUnknown: return "UnknowAttribute";
	case FbxNodeAttribute::eNull: return "Null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eMesh: return "Mesh";
	case FbxNodeAttribute::eSkeleton: return "Skeleton";
	case FbxNodeAttribute::eNurbs: return "Nurbs";
	case FbxNodeAttribute::eCamera: return "Path";
	case FbxNodeAttribute::eShape: return "Shape";
	default: return "UnknowAttribute";

	}
}

void printAttribute(FbxNodeAttribute* pattribute)
{
	if (!pattribute)
	{
		return;
	}

	FbxString typeName = GetAttributeTypeName(pattribute->GetAttributeType());
	FbxString attrName = pattribute->GetName();
	printTabs();

	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

void printNode(FbxNode* pnode)
{
	printTabs();

	const char* nodeName = pnode->GetName();

	FbxDouble3 translation = pnode->LclTranslation.Get();
	FbxDouble3 rotation = pnode->LclRotation.Get();
	FbxDouble3 scaling = pnode->LclScaling.Get();

	printf("<node name='%s' translation='(%f, %f, %f)', rotation='(%f, %f, %f)', scaling='(%f, %f, %f)'>\n",
		nodeName,
		translation[0], translation[1], translation[2],
		rotation[0], rotation[1], rotation[2],
		scaling[0], scaling[1], scaling[2]);

	numTabs++;

	for (int i = 0; i < pnode->GetNodeAttributeCount(); i++)
	{
		printAttribute(pnode->GetNodeAttributeByIndex(i));
	}

	for (int j = 0; j < pnode->GetChildCount(); j++)
	{
		printNode(pnode->GetChild(j));
	}

	numTabs--;
	printTabs();
	printf("</node>\n");
}

int main(int argc, char** argv)
{
	const char* filename = "test.fbx";

	FbxManager* pSdkManager = FbxManager::Create();
	FbxIOSettings* pFbxIOSettings = FbxIOSettings::Create(pSdkManager, filename);

	pSdkManager->SetIOSettings(pFbxIOSettings);

	FbxImporter* pImporter = FbxImporter::Create(pSdkManager, "");

	if (!pImporter->Initialize(filename, -1, pSdkManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed");
		printf("Error returned :%s\n\n", pImporter->GetStatus().GetErrorString());
		return -1;
	}

	FbxScene* pScene = FbxScene::Create(pSdkManager, "city1");

	pImporter->Import(pScene);

	pImporter->Destroy();

	FbxNode* pRootNode = pScene->GetRootNode();
	if (pRootNode)
	{
		int nodeCount = pRootNode->GetChildCount();
		for (int i = 0; i < nodeCount; i++)
		{
			printNode(pRootNode->GetChild(i));
		}
	}
	pSdkManager->Destroy();

	system("pause");
	return 0;
}
