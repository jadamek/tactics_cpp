//----------------------------------------------------------------------------
// - Resource Manager Destructor
//----------------------------------------------------------------------------
template <typename Type>
ResourceManager<Type>::~ResourceManager() {
    for(auto record : files_)
    {
		delete record.second;        
    }
}

//----------------------------------------------------------------------------
// - Load Resource (Safe)
//----------------------------------------------------------------------------
// * filepath : path to the resource to be loaded and returned
// Loads the resource if it is not already present
//----------------------------------------------------------------------------
template <typename Type>
const Type& ResourceManager<Type>::load(const std::string& filepath) {
	Type* resource;

	auto record = files_.find(filepath);

	if (record == files_.end()) {
		resource = new Type;
		resource->loadFromFile(filepath);
		files_[filepath] = resource;
	}
	else
		resource = record->second;

	return *resource;
}

//----------------------------------------------------------------------------
// - Retrieve (Unsafe)
//----------------------------------------------------------------------------
// * filepath : original path of the resource to be retrieved
// Assumes the resource has already been loaded and returns it
//----------------------------------------------------------------------------
template <typename Type>
const Type& ResourceManager<Type>::retrieve(const std::string& filepath) {
	return *files_[filepath];
}

//----------------------------------------------------------------------------
// - Unload (Filepath)
//----------------------------------------------------------------------------
// * filepath : original path of the file which should be deleted and removed
//----------------------------------------------------------------------------
template <typename Type>
void ResourceManager<Type>::unload(const std::string& filepath) {

    auto record = files_.find(filepath);
    
	if (record != files_.end()) {
		delete record->second;
		files_.erase(record);
	}
}

//----------------------------------------------------------------------------
// - Unload (Resource)
//----------------------------------------------------------------------------
// * resource : pointer to the resource which was loaded and should now be
// deleted and removed
//----------------------------------------------------------------------------
template <typename Type>
void ResourceManager<Type>::unload(const Type* resource) {    
    for (auto record = files_.begin(); record != files_.end(); record++) {
		if (record->second == resource) {
			delete record->second;
			files_.erase(record);
			break;
		}
	}
}